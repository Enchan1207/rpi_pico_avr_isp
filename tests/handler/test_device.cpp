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

TEST_F(DeviceHandlerTest, HandleGetParameterHwVersion) {
    parserCtx.command = STK500_CMD_GET_PARAMETER;
    parserCtx.expectedArgumentsLength = 1;
    parserCtx.receivedArgumentsLength = 1;
    parserCtx.arguments[0] = 0x80;  // PARM_STK_HW_VER

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0x02, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(DeviceHandlerTest, HandleGetParameterSwMajor) {
    parserCtx.command = STK500_CMD_GET_PARAMETER;
    parserCtx.expectedArgumentsLength = 1;
    parserCtx.receivedArgumentsLength = 1;
    parserCtx.arguments[0] = 0x81;  // PARM_STK_SW_MAJOR

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0x01, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(DeviceHandlerTest, HandleGetParameterSwMinor) {
    parserCtx.command = STK500_CMD_GET_PARAMETER;
    parserCtx.expectedArgumentsLength = 1;
    parserCtx.receivedArgumentsLength = 1;
    parserCtx.arguments[0] = 0x82;  // PARM_STK_SW_MINOR

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0x00, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(DeviceHandlerTest, HandleGetParameterUnknown) {
    parserCtx.command = STK500_CMD_GET_PARAMETER;
    parserCtx.expectedArgumentsLength = 1;
    parserCtx.receivedArgumentsLength = 1;
    parserCtx.arguments[0] = 0xFF;  // 不明なパラメータ

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0xFF, STK500_RESP_FAILED};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(DeviceHandlerTest, HandleSetParameterReadOnly) {
    parserCtx.command = STK500_CMD_SET_PARAMETER;
    parserCtx.expectedArgumentsLength = 2;
    parserCtx.receivedArgumentsLength = 2;
    parserCtx.arguments[0] = 0x80;  // PARM_STK_HW_VER (読み取り専用)
    parserCtx.arguments[1] = 0x05;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0x80, STK500_RESP_FAILED};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(DeviceHandlerTest, HandleSetParameterWritable) {
    parserCtx.command = STK500_CMD_SET_PARAMETER;
    parserCtx.expectedArgumentsLength = 2;
    parserCtx.receivedArgumentsLength = 2;
    parserCtx.arguments[0] = 0x83;  // PARM_STK_LEDS (読み書き可能)
    parserCtx.arguments[1] = 0x01;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(DeviceHandlerTest, HandleSetParameterUnknown) {
    parserCtx.command = STK500_CMD_SET_PARAMETER;
    parserCtx.expectedArgumentsLength = 2;
    parserCtx.receivedArgumentsLength = 2;
    parserCtx.arguments[0] = 0xFF;  // 不明なパラメータ
    parserCtx.arguments[1] = 0x01;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0xFF, STK500_RESP_FAILED};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

}  // namespace
