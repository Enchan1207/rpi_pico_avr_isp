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

    /// モック関数のためのデータ
    static uint8_t mockData[] = {STK500_CMD_GET_SYNC, 0x20};  // GET_SYNC + EOP
    static size_t mockIndex = 0;

    /// モック関数: データを順次返す
    auto mockReadFunc = [](uint8_t* data) -> bool {
        if (mockIndex < sizeof(mockData)) {
            *data = mockData[mockIndex++];
            return true;
        }
        return false;  // データなし
    };

    /// 初期状態の確認
    EXPECT_EQ(context.state, PARSER_READY);

    /// 1回目の処理: コマンド受信
    ParserState state1 = processParserInput(&context, mockReadFunc);
    EXPECT_EQ(state1, PARSER_EXPECTS_EOP);  // 引数なしなので直接EOP待機
    EXPECT_EQ(context.command, STK500_CMD_GET_SYNC);
    EXPECT_EQ(context.expectedArgumentsLength, 0);
    EXPECT_EQ(context.receivedArgumentsLength, 0);

    /// 2回目の処理: EOP受信
    ParserState state2 = processParserInput(&context, mockReadFunc);
    EXPECT_EQ(state2, PARSER_ACCEPTED);  // コマンド受理完了

    /// モックデータをリセット
    mockIndex = 0;
}
