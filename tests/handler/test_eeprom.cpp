#include "common.h"

using namespace handler_test;

namespace {

class EepromHandlerTest : public HandlerTestBase {
   protected:
    void SetUp() override {
        HandlerTestBase::SetUp();
        parserCtx.state = PARSER_ACCEPTED;
        // ATmega328PのEEPROMページサイズを設定（4バイト）
        handlerCtx.deviceInfo.eepromPageSize = 4;
    }
};

TEST_F(EepromHandlerTest, HandleProgDataCommand) {
    parserCtx.command = STK500_CMD_PROG_DATA;
    parserCtx.expectedArgumentsLength = 1;
    parserCtx.receivedArgumentsLength = 1;
    parserCtx.arguments[0] = 0xA5;  // 書き込むデータ

    // アドレスを設定
    handlerCtx.currentAddress = 0x0123;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
    EXPECT_EQ(handlerCtx.currentAddress, 0x0124);  // アドレス自動インクリメントを確認
}

TEST_F(EepromHandlerTest, HandleReadDataCommand) {
    mockIspTransferReturnValue = 0x42;
    parserCtx.command = STK500_CMD_READ_DATA;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    // アドレスを設定
    handlerCtx.currentAddress = 0x0456;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0x42, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
    EXPECT_EQ(handlerCtx.currentAddress, 0x0457);  // アドレス自動インクリメントを確認
}

TEST_F(EepromHandlerTest, HandleProgDataWithZeroAddress) {
    parserCtx.command = STK500_CMD_PROG_DATA;
    parserCtx.expectedArgumentsLength = 1;
    parserCtx.receivedArgumentsLength = 1;
    parserCtx.arguments[0] = 0xFF;

    // アドレス0
    handlerCtx.currentAddress = 0x0000;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
    EXPECT_EQ(handlerCtx.currentAddress, 0x0001);  // アドレス自動インクリメントを確認
}

TEST_F(EepromHandlerTest, HandleProgDataPageBoundary) {
    parserCtx.command = STK500_CMD_PROG_DATA;
    parserCtx.expectedArgumentsLength = 1;
    parserCtx.receivedArgumentsLength = 1;
    parserCtx.arguments[0] = 0xAB;

    // 4バイトページの最後のバイト（ページ境界）
    handlerCtx.currentAddress = 7;  // 4バイトページの2ページ目の最後

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
    EXPECT_EQ(handlerCtx.currentAddress, 8);  // 次のページの先頭
}

TEST_F(EepromHandlerTest, HandleProgDataDifferentPageSizes) {
    // 8バイトページサイズでテスト
    handlerCtx.deviceInfo.eepromPageSize = 8;
    
    parserCtx.command = STK500_CMD_PROG_DATA;
    parserCtx.expectedArgumentsLength = 1;
    parserCtx.receivedArgumentsLength = 1;
    parserCtx.arguments[0] = 0x11;

    // アドレス10（8バイトページの2ページ目の3番目）
    handlerCtx.currentAddress = 10;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
    EXPECT_EQ(handlerCtx.currentAddress, 11);  // アドレス自動インクリメント
}

}  // namespace