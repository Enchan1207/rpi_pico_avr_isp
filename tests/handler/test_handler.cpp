#include "handler_test_common.h"

namespace handler_test {

// ハンドラの異常系テスト
class HandlerErrorTest : public HandlerTestBase {};

// パーサエラー状態のテスト
TEST_F(HandlerErrorTest, HandleParserError) {
    // パーサコンテキストを設定（エラー状態をシミュレート）
    parserCtx.state = PARSER_ERROR;

    // handleCommandを実行
    handleCommand(&parserCtx, &handlerCtx);

    // 期待されるレスポンス: STK500_RESP_NO_SYNC (0x15)
    std::vector<uint8_t> expectedResponse = {STK500_RESP_NO_SYNC};

    // レスポンスが正しくキャプチャされたかを確認
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

// 不明なコマンドのテスト
TEST_F(HandlerErrorTest, HandleUnknownCommand) {
    // パーサコンテキストを設定（不明なコマンド状態をシミュレート）
    parserCtx.state = PARSER_UNKNOWN;

    // handleCommandを実行
    handleCommand(&parserCtx, &handlerCtx);

    // 期待されるレスポンス: STK500_RESP_UNKNOWN (0x12)
    std::vector<uint8_t> expectedResponse = {STK500_RESP_UNKNOWN};

    // レスポンスが正しくキャプチャされたかを確認
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

}  // namespace