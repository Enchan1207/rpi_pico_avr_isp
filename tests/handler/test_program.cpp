#include "handler_test_common.h"

using namespace handler_test;

namespace {

class ProgramHandlerTest : public HandlerTestBase {
   protected:
    void SetUp() override {
        HandlerTestBase::SetUp();
        parserCtx.state = PARSER_ACCEPTED;
    }
};

TEST_F(ProgramHandlerTest, HandleLoadAddressCommand) {
    parserCtx.command = STK500_CMD_LOAD_ADDRESS;
    parserCtx.expectedArgumentsLength = 2;
    parserCtx.receivedArgumentsLength = 2;
    parserCtx.arguments[0] = 0x12;  // 上位バイト
    parserCtx.arguments[1] = 0x34;  // 下位バイト

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
    EXPECT_EQ(handlerCtx.currentAddress, 0x1234);
}

TEST_F(ProgramHandlerTest, HandleCheckAutoIncCommand) {
    parserCtx.command = STK500_CMD_CHECK_AUTOINC;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

}  // namespace