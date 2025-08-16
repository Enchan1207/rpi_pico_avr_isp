#include "handler_test_common.h"

using namespace handler_test;

namespace {

// Syncハンドラの正常系テスト
class SyncHandlerTest : public HandlerTestBase {};

// STK500_CMD_GET_SYNCコマンドの正常系テスト
TEST_F(SyncHandlerTest, HandleGetSyncCommand) {
    // パーサコンテキストを設定（GET_SYNCコマンドが受理された状態をシミュレート）
    parserCtx.state = PARSER_ACCEPTED;
    parserCtx.command = STK500_CMD_GET_SYNC;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    // handleCommandを実行
    handleCommand(&parserCtx, &handlerCtx);

    // 期待されるレスポンス: STK500_RESP_IN_SYNC (0x14), STK500_RESP_OK (0x10)
    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};

    // レスポンスが正しくキャプチャされたかを確認
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

// STK500_CMD_GET_SIGN_ONコマンドの正常系テスト
TEST_F(SyncHandlerTest, HandleGetSignOnCommand) {
    // パーサコンテキストを設定（GET_SIGN_ONコマンドが受理された状態をシミュレート）
    parserCtx.state = PARSER_ACCEPTED;
    parserCtx.command = STK500_CMD_GET_SIGN_ON;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    // handleCommandを実行
    handleCommand(&parserCtx, &handlerCtx);

    // 期待されるレスポンス: STK500_RESP_IN_SYNC, "AVR STK", STK500_RESP_OK
    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC};
    
    const char* expectedMessage = "AVR STK";
    for (size_t i = 0; i < std::strlen(expectedMessage); ++i) {
        expectedResponse.push_back(expectedMessage[i]);
    }
    expectedResponse.push_back(STK500_RESP_OK);

    // レスポンスが正しくキャプチャされたかを確認
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

}  // namespace