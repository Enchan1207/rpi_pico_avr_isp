#include <gtest/gtest.h>
#include <stdbool.h>
#include <stdint.h>

#include "core/parser.h"

TEST(ParserTest, LengthForValidCommandWithNoArgs) {
    auto actual = getCommandArgumentsLength(STK500_CMD_GET_SYNC);
    EXPECT_EQ(actual, 0);
}

TEST(ParserTest, LengthForValidCommandWithArgs) {
    auto actual = getCommandArgumentsLength(STK500_CMD_SET_PARAMETER);
    EXPECT_EQ(actual, 2);
}

TEST(ParserTest, LengthForInvalidCommand) {
    auto actual = getCommandArgumentsLength(static_cast<Stk500Command>(0xFF));
    EXPECT_EQ(actual, 0);
}

TEST(ParserTest, ParsePayloadWithNoArguments) {
    parser_context_t context;
    initParserContext(&context);

    // ペイロードを返すモック
    static uint8_t mockData[] = {STK500_CMD_GET_SYNC, STK500_EOP};
    static size_t mockIndex = 0;

    auto mockReadFunc = [](uint8_t* data) -> bool {
        if (mockIndex >= sizeof(mockData)) {
            return false;
        }

        *data = mockData[mockIndex++];
        return true;
    };

    EXPECT_EQ(context.state, PARSER_READY);

    /// 1回目: コマンド受信
    ParserState state1 = processParserInput(&context, mockReadFunc);
    EXPECT_EQ(state1, PARSER_EXPECTS_EOP);
    EXPECT_EQ(context.command, STK500_CMD_GET_SYNC);
    EXPECT_EQ(context.expectedArgumentsLength, 0);
    EXPECT_EQ(context.receivedArgumentsLength, 0);

    /// 2回目: 受理
    ParserState state2 = processParserInput(&context, mockReadFunc);
    EXPECT_EQ(state2, PARSER_ACCEPTED);

    mockIndex = 0;
}

TEST(ParserTest, ParsePayloadWithArguments) {
    parser_context_t context;
    initParserContext(&context);

    // ペイロードを返すモック
    static uint8_t mockData[] = {STK500_CMD_LOAD_ADDRESS, 0x34, 0x12, STK500_EOP};
    static size_t mockIndex = 0;

    auto mockReadFunc = [](uint8_t* data) -> bool {
        if (mockIndex >= sizeof(mockData)) {
            return false;
        }

        *data = mockData[mockIndex++];
        return true;
    };

    EXPECT_EQ(context.state, PARSER_READY);

    /// 1回目: コマンド受信
    ParserState state1 = processParserInput(&context, mockReadFunc);
    EXPECT_EQ(state1, PARSER_RECEIVE_ARGS);
    EXPECT_EQ(context.command, STK500_CMD_LOAD_ADDRESS);
    EXPECT_EQ(context.expectedArgumentsLength, 2);

    /// 2回目: 状態維持
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_RECEIVE_ARGS);

    /// 3回目: 終端待ちに遷移
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_EXPECTS_EOP);

    /// 4回目: 受理
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_ACCEPTED);

    EXPECT_EQ(context.receivedArgumentsLength, 2);
    EXPECT_EQ(context.arguments[0], 0x34);
    EXPECT_EQ(context.arguments[1], 0x12);

    mockIndex = 0;
}

TEST(ParserTest, ParsePayloadForProgPage) {
    parser_context_t context;
    initParserContext(&context);

    // ペイロードを返すモック
    static uint8_t mockData[] = {STK500_CMD_PROG_PAGE, 0x00, 0x05, 0x46, 0x01, 0x23, 0x45, 0x67, 0x89, STK500_EOP};
    static size_t mockIndex = 0;

    auto mockReadFunc = [](uint8_t* data) -> bool {
        if (mockIndex >= sizeof(mockData)) {
            return false;
        }

        *data = mockData[mockIndex++];
        return true;
    };

    EXPECT_EQ(context.state, PARSER_READY);

    /// 1回目: コマンド受信
    ParserState state1 = processParserInput(&context, mockReadFunc);
    EXPECT_EQ(state1, PARSER_RECEIVE_ARGS);
    EXPECT_EQ(context.command, STK500_CMD_PROG_PAGE);
    EXPECT_EQ(context.expectedArgumentsLength, 3);

    /// 2回目: 状態維持
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_RECEIVE_ARGS);

    /// 3回目: 状態維持、総コマンド長再設定
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(context.expectedArgumentsLength, 8);

    // 1+5byte分のデータを追加で読み込み、終端待ちに遷移
    processParserInput(&context, mockReadFunc);
    processParserInput(&context, mockReadFunc);
    processParserInput(&context, mockReadFunc);
    processParserInput(&context, mockReadFunc);
    processParserInput(&context, mockReadFunc);
    EXPECT_EQ(context.state, PARSER_RECEIVE_ARGS);
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_EXPECTS_EOP);

    /// 受理
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_ACCEPTED);

    EXPECT_EQ(context.receivedArgumentsLength, 8);
    EXPECT_EQ(context.arguments[0], 0x00);
    EXPECT_EQ(context.arguments[1], 0x05);
    EXPECT_EQ(context.arguments[2], 0x46);
    EXPECT_EQ(context.arguments[3], 0x01);
    EXPECT_EQ(context.arguments[4], 0x23);
    EXPECT_EQ(context.arguments[5], 0x45);
    EXPECT_EQ(context.arguments[6], 0x67);
    EXPECT_EQ(context.arguments[7], 0x89);

    mockIndex = 0;
}

TEST(ParserTest, ParsePayloadForUniversalMulti) {
    parser_context_t context;
    initParserContext(&context);

    // ペイロードを返すモック
    static uint8_t mockData[] = {STK500_CMD_UNIVERSAL_MULTI, 0x05, 0x01, 0x23, 0x45, 0x67, 0x89, STK500_EOP};
    static size_t mockIndex = 0;

    auto mockReadFunc = [](uint8_t* data) -> bool {
        if (mockIndex >= sizeof(mockData)) {
            return false;
        }

        *data = mockData[mockIndex++];
        return true;
    };

    EXPECT_EQ(context.state, PARSER_READY);

    /// 1回目: コマンド受信
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(context.command, STK500_CMD_UNIVERSAL_MULTI);
    EXPECT_EQ(context.expectedArgumentsLength, 1);

    /// 2回目: 状態維持、総コマンド長再設定
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(context.expectedArgumentsLength, 6);

    // 5byte分のデータを追加で読み込み、終端待ちに遷移
    processParserInput(&context, mockReadFunc);
    processParserInput(&context, mockReadFunc);
    processParserInput(&context, mockReadFunc);
    processParserInput(&context, mockReadFunc);
    EXPECT_EQ(context.state, PARSER_RECEIVE_ARGS);
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_EXPECTS_EOP);

    /// 受理
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_ACCEPTED);

    EXPECT_EQ(context.receivedArgumentsLength, 6);
    EXPECT_EQ(context.arguments[0], 0x05);
    EXPECT_EQ(context.arguments[1], 0x01);
    EXPECT_EQ(context.arguments[2], 0x23);
    EXPECT_EQ(context.arguments[3], 0x45);
    EXPECT_EQ(context.arguments[4], 0x67);
    EXPECT_EQ(context.arguments[5], 0x89);

    mockIndex = 0;
}

TEST(ParserTest, ParseUnknownCommand) {
    parser_context_t context;
    initParserContext(&context);

    // ペイロードを返すモック
    static uint8_t mockData[] = {0xFF, STK500_EOP};
    static size_t mockIndex = 0;

    auto mockReadFunc = [](uint8_t* data) -> bool {
        if (mockIndex >= sizeof(mockData)) {
            return false;
        }

        *data = mockData[mockIndex++];
        return true;
    };

    EXPECT_EQ(context.state, PARSER_READY);

    /// 1回目: コマンド受信、終端待ちに遷移
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_EXPECTS_EOP);
    EXPECT_EQ(context.command, 0xFF);

    /// 2回目: エラー
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_UNKNOWN);

    mockIndex = 0;
}

TEST(ParserTest, TerminateWhileParse) {
    parser_context_t context;
    initParserContext(&context);

    // ペイロードを返すモック
    static uint8_t mockData[] = {0xFF};
    static size_t mockIndex = 0;

    auto mockReadFunc = [](uint8_t* data) -> bool {
        if (mockIndex >= sizeof(mockData)) {
            return false;
        }

        *data = mockData[mockIndex++];
        return true;
    };

    EXPECT_EQ(context.state, PARSER_READY);

    /// 1回目: コマンド受信、終端待ちに遷移
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_EXPECTS_EOP);
    EXPECT_EQ(context.command, 0xFF);

    /// 2回目: エラー
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_ERROR);

    mockIndex = 0;
}

TEST(ParserTest, TerminateWhileParseWithArgs) {
    parser_context_t context;
    initParserContext(&context);

    // ペイロードを返すモック
    static uint8_t mockData[] = {STK500_CMD_UNIVERSAL, 0x01, 0x23, 0x45};
    static size_t mockIndex = 0;

    auto mockReadFunc = [](uint8_t* data) -> bool {
        if (mockIndex >= sizeof(mockData)) {
            return false;
        }

        *data = mockData[mockIndex++];
        return true;
    };

    EXPECT_EQ(context.state, PARSER_READY);

    /// 1回目: コマンド受信、終端待ちに遷移
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(context.command, STK500_CMD_UNIVERSAL);

    /// 2~4回目: 状態維持
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_RECEIVE_ARGS);

    /// 5回目でEOPを受け取れなかった
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_ERROR);

    mockIndex = 0;
}

TEST(ParserTest, TerminateWhileParseProgPage) {
    parser_context_t context;
    initParserContext(&context);

    // ペイロードを返すモック
    static uint8_t mockData[] = {STK500_CMD_PROG_PAGE, 0x00};
    static size_t mockIndex = 0;

    auto mockReadFunc = [](uint8_t* data) -> bool {
        if (mockIndex >= sizeof(mockData)) {
            return false;
        }

        *data = mockData[mockIndex++];
        return true;
    };

    EXPECT_EQ(context.state, PARSER_READY);

    /// 1回目: コマンド受信、終端待ちに遷移
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_RECEIVE_ARGS);
    EXPECT_EQ(context.command, STK500_CMD_PROG_PAGE);

    /// 2回目: 状態維持
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_RECEIVE_ARGS);

    /// 引数の受信中にエラー
    EXPECT_EQ(processParserInput(&context, mockReadFunc), PARSER_ERROR);

    mockIndex = 0;
}
