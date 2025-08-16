#include "common.h"

using namespace handler_test;

namespace {

class DeviceHandlerTest : public HandlerTestBase {
   protected:
    void SetUp() override {
        HandlerTestBase::SetUp();
        parserCtx.state = PARSER_ACCEPTED;
    }
};

TEST_F(DeviceHandlerTest, HandleReadSignCommand) {
    mockIspTransferReturnValue = 0x1E;  // ATmega328Pの場合
    parserCtx.command = STK500_CMD_READ_SIGN;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0x1E, 0x1E, 0x1E, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(DeviceHandlerTest, HandleReadOscCalCommand) {
    mockIspTransferReturnValue = 0x80;
    parserCtx.command = STK500_CMD_READ_OSC_CAL;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0x80, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(DeviceHandlerTest, HandleReadOscCalExtCommand) {
    mockIspTransferReturnValue = 0x85;
    parserCtx.command = STK500_CMD_READ_OSC_CAL_EXT;
    parserCtx.expectedArgumentsLength = 1;
    parserCtx.receivedArgumentsLength = 1;
    parserCtx.arguments[0] = 0x01;  // アドレス

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0x85, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

}  // namespace