#include "handler_test_common.h"

using namespace handler_test;

namespace {

class UniversalHandlerTest : public HandlerTestBase {
   protected:
    void SetUp() override {
        HandlerTestBase::SetUp();
        parserCtx.state = PARSER_ACCEPTED;
    }
};

TEST_F(UniversalHandlerTest, HandleUniversalCommand) {
    parserCtx.command = STK500_CMD_UNIVERSAL;
    parserCtx.expectedArgumentsLength = 4;
    parserCtx.receivedArgumentsLength = 4;
    parserCtx.arguments[0] = 0xAC;  // Programming Enable command
    parserCtx.arguments[1] = 0x53;
    parserCtx.arguments[2] = 0x00;
    parserCtx.arguments[3] = 0x00;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0x00, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(UniversalHandlerTest, HandleUniversalCommandInsufficientArgs) {
    parserCtx.command = STK500_CMD_UNIVERSAL;
    parserCtx.expectedArgumentsLength = 4;
    parserCtx.receivedArgumentsLength = 3;
    parserCtx.arguments[0] = 0xAC;
    parserCtx.arguments[1] = 0x53;
    parserCtx.arguments[2] = 0x00;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_FAILED};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(UniversalHandlerTest, HandleUniversalMultiCommand) {
    parserCtx.command = STK500_CMD_UNIVERSAL_MULTI;
    parserCtx.expectedArgumentsLength = 9;
    parserCtx.receivedArgumentsLength = 9;
    parserCtx.arguments[0] = 8;  // データ長

    // 1つ目のISPコマンド
    parserCtx.arguments[1] = 0xAC;
    parserCtx.arguments[2] = 0x53;
    parserCtx.arguments[3] = 0x00;
    parserCtx.arguments[4] = 0x00;

    // 2つ目のISPコマンド
    parserCtx.arguments[5] = 0x30;
    parserCtx.arguments[6] = 0x00;
    parserCtx.arguments[7] = 0x00;
    parserCtx.arguments[8] = 0x00;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(UniversalHandlerTest, HandleUniversalMultiCommandInvalidParameterLength) {
    parserCtx.command = STK500_CMD_UNIVERSAL_MULTI;
    parserCtx.expectedArgumentsLength = 4;
    parserCtx.receivedArgumentsLength = 4;
    parserCtx.arguments[0] = 3;  // 3バイト（4バイト単位でない）
    parserCtx.arguments[1] = 0xAC;
    parserCtx.arguments[2] = 0x53;
    parserCtx.arguments[3] = 0x00;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_FAILED};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

}  // namespace