#include "common.h"

using namespace handler_test;

namespace {

class FlashHandlerTest : public HandlerTestBase {
   protected:
    void SetUp() override {
        HandlerTestBase::SetUp();
        parserCtx.state = PARSER_ACCEPTED;
        // ATmega328Pのページサイズ（128バイト = 64ワード）を設定
        handlerCtx.deviceInfo.pageSize = 64;
    }
};

TEST_F(FlashHandlerTest, HandleProgFlashCommand) {
    parserCtx.command = STK500_CMD_PROG_FLASH;
    parserCtx.expectedArgumentsLength = 2;
    parserCtx.receivedArgumentsLength = 2;
    parserCtx.arguments[0] = 0xA5;  // flash_low
    parserCtx.arguments[1] = 0x5A;  // flash_high

    // アドレスを設定
    handlerCtx.currentAddress = 0x0123;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
    EXPECT_EQ(handlerCtx.currentAddress, 0x0124);  // アドレス自動インクリメントを確認
}

TEST_F(FlashHandlerTest, HandleReadFlashCommand) {
    mockIspTransferReturnValue = 0x42;
    parserCtx.command = STK500_CMD_READ_FLASH;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    // アドレスを設定
    handlerCtx.currentAddress = 0x0456;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0x42, 0x42, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
    EXPECT_EQ(handlerCtx.currentAddress, 0x0457);  // アドレス自動インクリメントを確認
}

TEST_F(FlashHandlerTest, HandleProgFlashWithZeroAddress) {
    parserCtx.command = STK500_CMD_PROG_FLASH;
    parserCtx.expectedArgumentsLength = 2;
    parserCtx.receivedArgumentsLength = 2;
    parserCtx.arguments[0] = 0xFF;  // flash_low
    parserCtx.arguments[1] = 0x00;  // flash_high

    // アドレス0
    handlerCtx.currentAddress = 0x0000;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
    EXPECT_EQ(handlerCtx.currentAddress, 0x0001);  // アドレス自動インクリメントを確認
}

TEST_F(FlashHandlerTest, HandleProgFlashPageBoundary) {
    parserCtx.command = STK500_CMD_PROG_FLASH;
    parserCtx.expectedArgumentsLength = 2;
    parserCtx.receivedArgumentsLength = 2;
    parserCtx.arguments[0] = 0xAB;  // flash_low
    parserCtx.arguments[1] = 0xCD;  // flash_high

    // ページ境界（64ワードページの最後のワード）
    handlerCtx.currentAddress = 63;  // 0x3F

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
    EXPECT_EQ(handlerCtx.currentAddress, 64);  // 次のページの先頭
}

TEST_F(FlashHandlerTest, HandleProgFlashDifferentPageSizes) {
    // 32ワードページサイズでテスト
    handlerCtx.deviceInfo.pageSize = 32;
    
    parserCtx.command = STK500_CMD_PROG_FLASH;
    parserCtx.expectedArgumentsLength = 2;
    parserCtx.receivedArgumentsLength = 2;
    parserCtx.arguments[0] = 0x11;  // flash_low
    parserCtx.arguments[1] = 0x22;  // flash_high

    // アドレス50（32ワードページの2ページ目の18番目）
    handlerCtx.currentAddress = 50;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
    EXPECT_EQ(handlerCtx.currentAddress, 51);  // アドレス自動インクリメント
}

}  // namespace