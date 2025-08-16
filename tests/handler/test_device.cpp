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

TEST_F(DeviceHandlerTest, HandleSetDeviceCommand) {
    parserCtx.command = STK500_CMD_SET_DEVICE;
    parserCtx.expectedArgumentsLength = 20;
    parserCtx.receivedArgumentsLength = 20;
    
    // デバイス設定データを準備
    parserCtx.arguments[0] = 0x1E;   // deviceCode (ATmega328P)
    parserCtx.arguments[1] = 0x00;   // revision (未使用)
    parserCtx.arguments[2] = 0x00;   // progtype (未使用)
    parserCtx.arguments[3] = 0x00;   // parmode (未使用)
    parserCtx.arguments[4] = 0x00;   // polling (未使用)
    parserCtx.arguments[5] = 0x00;   // selftimed (未使用)
    parserCtx.arguments[6] = 0x03;   // lockBytes
    parserCtx.arguments[7] = 0x03;   // fuseBytes
    parserCtx.arguments[8] = 0x00;   // flashpollval1 (未使用)
    parserCtx.arguments[9] = 0x00;   // flashpollval2 (未使用)
    parserCtx.arguments[10] = 0x00;  // eeprompollval1 (未使用)
    parserCtx.arguments[11] = 0x00;  // eeprompollval2 (未使用)
    parserCtx.arguments[12] = 0x00;  // pageSizeHigh
    parserCtx.arguments[13] = 0x80;  // pageSizeLow (128 bytes)
    parserCtx.arguments[14] = 0x04;  // eepromSizeHigh
    parserCtx.arguments[15] = 0x00;  // eepromSizeLow (1024 bytes)
    parserCtx.arguments[16] = 0x00;  // flashSize4
    parserCtx.arguments[17] = 0x00;  // flashSize3
    parserCtx.arguments[18] = 0x80;  // flashSize2
    parserCtx.arguments[19] = 0x00;  // flashSize1 (32KB)

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);

    // デバイス情報が正しく設定されたか確認
    EXPECT_EQ(handlerCtx.deviceInfo.deviceCode, 0x1E);
    EXPECT_EQ(handlerCtx.deviceInfo.lockBytesLength, 0x03);
    EXPECT_EQ(handlerCtx.deviceInfo.fuseBytesLength, 0x03);
    EXPECT_EQ(handlerCtx.deviceInfo.pageSize, 0x0080);
    EXPECT_EQ(handlerCtx.deviceInfo.eepromSize, 0x0400);
    EXPECT_EQ(handlerCtx.deviceInfo.flashSize, 0x00008000);
}

TEST_F(DeviceHandlerTest, HandleSetDeviceExtCommand) {
    parserCtx.command = STK500_CMD_SET_DEVICE_EXT;
    parserCtx.expectedArgumentsLength = 4;
    parserCtx.receivedArgumentsLength = 4;
    
    parserCtx.arguments[0] = 0x05;   // commandSize
    parserCtx.arguments[1] = 0x04;   // eepromPageSize
    parserCtx.arguments[2] = 0x00;   // signalpagel (未使用)
    parserCtx.arguments[3] = 0x00;   // signalbs2 (未使用)

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);

    // デバイス拡張情報が正しく設定されたか確認
    EXPECT_EQ(handlerCtx.deviceInfo.commandSize, 0x05);
    EXPECT_EQ(handlerCtx.deviceInfo.eepromPageSize, 0x04);
}

}  // namespace
