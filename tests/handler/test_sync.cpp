#include "common.h"

using namespace handler_test;

namespace {

class SyncHandlerTest : public HandlerTestBase {};

TEST_F(SyncHandlerTest, HandleGetSyncCommand) {
    parserCtx.state = PARSER_ACCEPTED;
    parserCtx.command = STK500_CMD_GET_SYNC;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(SyncHandlerTest, HandleGetSignOnCommand) {
    parserCtx.state = PARSER_ACCEPTED;
    parserCtx.command = STK500_CMD_GET_SIGN_ON;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC};
    
    const char* expectedMessage = "AVR STK";
    for (size_t i = 0; i < std::strlen(expectedMessage); ++i) {
        expectedResponse.push_back(expectedMessage[i]);
    }
    expectedResponse.push_back(STK500_RESP_OK);

    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

}  // namespace