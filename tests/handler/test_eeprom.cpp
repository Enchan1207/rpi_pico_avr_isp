#include "common.h"

using namespace handler_test;

namespace {

class EepromHandlerTest : public HandlerTestBase {
   protected:
    void SetUp() override {
        HandlerTestBase::SetUp();
        parserCtx.state = PARSER_ACCEPTED;
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
}

}  // namespace