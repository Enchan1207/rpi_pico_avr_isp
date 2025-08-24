#include <gtest/gtest.h>
#include <vector>
#include <cstring>

extern "C" {
#include "core/parser.h"
#include "core/stk500.h"
}

namespace {

// テスト用のモックデータリーダークラス
class MockDataReader {
public:
    explicit MockDataReader(const std::vector<uint8_t>& data) : data_(data), index_(0) {}

    bool readByte(uint8_t* data) {
        if (index_ >= data_.size()) {
            return false;
        }
        *data = data_[index_++];
        return true;
    }

    void reset() { index_ = 0; }

    static bool mockReadFunction(uint8_t* data) {
        return instance_->readByte(data);
    }

    static MockDataReader* instance_;

private:
    std::vector<uint8_t> data_;
    size_t index_;
};

MockDataReader* MockDataReader::instance_ = nullptr;

class ParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        initParserContext(&parserCtx, argumentsBuffer, sizeof(argumentsBuffer));
    }

    void setupMockData(const std::vector<uint8_t>& data) {
        mockReader_ = std::make_unique<MockDataReader>(data);
        MockDataReader::instance_ = mockReader_.get();
    }

    parser_context_t parserCtx;
    uint8_t argumentsBuffer[259];
    std::unique_ptr<MockDataReader> mockReader_;
};

TEST_F(ParserTest, LengthForValidCommandWithNoArgs) {
    auto actual = getCommandArgumentsLength(STK500_CMD_GET_SYNC);
    EXPECT_EQ(actual, 0);
}

TEST_F(ParserTest, LengthForValidCommandWithArgs) {
    auto actual = getCommandArgumentsLength(STK500_CMD_SET_PARAMETER);
    EXPECT_EQ(actual, 2);
}

TEST_F(ParserTest, LengthForInvalidCommand) {
    auto actual = getCommandArgumentsLength(static_cast<Stk500Command>(0xFF));
    EXPECT_EQ(actual, 0);
}

TEST_F(ParserTest, ParsePayloadWithNoArguments) {
    setupMockData({STK500_CMD_GET_SYNC, STK500_EOP});

    EXPECT_EQ(parserCtx.state, PARSER_READY);

    // 1回目: コマンド受信
    ParserState state1 = processParserInput(&parserCtx, MockDataReader::mockReadFunction);
    EXPECT_EQ(state1, PARSER_EXPECTS_EOP);
    EXPECT_EQ(parserCtx.command, STK500_CMD_GET_SYNC);
    EXPECT_EQ(parserCtx.expectedArgumentsLength, 0);
    EXPECT_EQ(parserCtx.receivedArgumentsLength, 0);

    // 2回目: 受理
    ParserState state2 = processParserInput(&parserCtx, MockDataReader::mockReadFunction);
    EXPECT_EQ(state2, PARSER_ACCEPTED);
}

TEST_F(ParserTest, ParsePayloadWithArguments) {
    setupMockData({STK500_CMD_LOAD_ADDRESS, 0x34, 0x12, STK500_EOP});

    EXPECT_EQ(parserCtx.state, PARSER_READY);

    // 1回目: コマンド受信
    ParserState state1 = processParserInput(&parserCtx, MockDataReader::mockReadFunction);
    EXPECT_EQ(state1, PARSER_RECEIVE_ARGS);
    EXPECT_EQ(parserCtx.command, STK500_CMD_LOAD_ADDRESS);
    EXPECT_EQ(parserCtx.expectedArgumentsLength, 2);

    // 2回目: 引数受信継続
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);

    // 3回目: 終端待ちに遷移
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_EXPECTS_EOP);

    // 4回目: 受理
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_ACCEPTED);

    EXPECT_EQ(parserCtx.receivedArgumentsLength, 2);
    EXPECT_EQ(parserCtx.arguments[0], 0x34);
    EXPECT_EQ(parserCtx.arguments[1], 0x12);
}

TEST_F(ParserTest, ParsePayloadForProgPage) {
    setupMockData({STK500_CMD_PROG_PAGE, 0x00, 0x05, 0x46, 0x01, 0x23, 0x45, 0x67, 0x89, STK500_EOP});

    EXPECT_EQ(parserCtx.state, PARSER_READY);

    // 1回目: コマンド受信
    ParserState state1 = processParserInput(&parserCtx, MockDataReader::mockReadFunction);
    EXPECT_EQ(state1, PARSER_RECEIVE_ARGS);
    EXPECT_EQ(parserCtx.command, STK500_CMD_PROG_PAGE);
    EXPECT_EQ(parserCtx.expectedArgumentsLength, 3);

    // 2回目: 引数受信継続
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);

    // 3回目: 引数受信継続、総コマンド長再設定
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(parserCtx.expectedArgumentsLength, 8);

    // 1+5byte分のデータを追加で読み込み、終端待ちに遷移
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);
    }
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_EXPECTS_EOP);

    // 受理
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_ACCEPTED);

    EXPECT_EQ(parserCtx.receivedArgumentsLength, 8);
    EXPECT_EQ(parserCtx.arguments[0], 0x00);
    EXPECT_EQ(parserCtx.arguments[1], 0x05);
    EXPECT_EQ(parserCtx.arguments[2], 0x46);
    EXPECT_EQ(parserCtx.arguments[3], 0x01);
    EXPECT_EQ(parserCtx.arguments[4], 0x23);
    EXPECT_EQ(parserCtx.arguments[5], 0x45);
    EXPECT_EQ(parserCtx.arguments[6], 0x67);
    EXPECT_EQ(parserCtx.arguments[7], 0x89);
}

TEST_F(ParserTest, ParsePayloadForUniversalMulti) {
    setupMockData({STK500_CMD_UNIVERSAL_MULTI, 0x05, 0x01, 0x23, 0x45, 0x67, 0x89, STK500_EOP});

    EXPECT_EQ(parserCtx.state, PARSER_READY);

    // 1回目: コマンド受信
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(parserCtx.command, STK500_CMD_UNIVERSAL_MULTI);
    EXPECT_EQ(parserCtx.expectedArgumentsLength, 1);

    // 2回目: 引数受信継続、総コマンド長再設定
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(parserCtx.expectedArgumentsLength, 6);

    // 5byte分のデータを追加で読み込み、終端待ちに遷移
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);
    }
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_EXPECTS_EOP);

    // 受理
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_ACCEPTED);

    EXPECT_EQ(parserCtx.receivedArgumentsLength, 6);
    EXPECT_EQ(parserCtx.arguments[0], 0x05);
    EXPECT_EQ(parserCtx.arguments[1], 0x01);
    EXPECT_EQ(parserCtx.arguments[2], 0x23);
    EXPECT_EQ(parserCtx.arguments[3], 0x45);
    EXPECT_EQ(parserCtx.arguments[4], 0x67);
    EXPECT_EQ(parserCtx.arguments[5], 0x89);
}

TEST_F(ParserTest, ParseUnknownCommand) {
    setupMockData({0xFF, STK500_EOP});

    EXPECT_EQ(parserCtx.state, PARSER_READY);

    // 1回目: コマンド受信、終端待ちに遷移
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_EXPECTS_EOP);
    EXPECT_EQ(parserCtx.command, 0xFF);

    // 2回目: エラー
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_UNKNOWN);
}

TEST_F(ParserTest, TerminateWhileParse) {
    setupMockData({0xFF});

    EXPECT_EQ(parserCtx.state, PARSER_READY);

    // 1回目: コマンド受信、終端待ちに遷移
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_EXPECTS_EOP);
    EXPECT_EQ(parserCtx.command, 0xFF);

    // 2回目: エラー（データなし）
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_ERROR);
}

TEST_F(ParserTest, TerminateWhileParseWithArgs) {
    setupMockData({STK500_CMD_UNIVERSAL, 0x01, 0x23, 0x45});

    EXPECT_EQ(parserCtx.state, PARSER_READY);

    // 1回目: コマンド受信、引数受信に遷移
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(parserCtx.command, STK500_CMD_UNIVERSAL);

    // 2~4回目: 引数受信継続
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);

    // 5回目: EOPを受け取れなかった
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_ERROR);
}

TEST_F(ParserTest, TerminateWhileParseProgPage) {
    setupMockData({STK500_CMD_PROG_PAGE, 0x00});

    EXPECT_EQ(parserCtx.state, PARSER_READY);

    // 1回目: コマンド受信、引数受信に遷移
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(parserCtx.command, STK500_CMD_PROG_PAGE);

    // 2回目: 引数受信継続
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_RECEIVE_ARGS);

    // 引数の受信中にエラー
    EXPECT_EQ(processParserInput(&parserCtx, MockDataReader::mockReadFunction), PARSER_ERROR);
}

}  // namespace