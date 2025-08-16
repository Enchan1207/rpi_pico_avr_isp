#include <gtest/gtest.h>

#include <cstring>
#include <vector>

extern "C" {
#include "core/handler.h"
#include "core/parser.h"
#include "core/stk500.h"
}

namespace {

uint8_t mockIspTransfer(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3, uint8_t cmd4) {
    return 0x00;
}

std::vector<uint8_t> capturedResponse;
void mockResponseWriter(const uint8_t* response, size_t length) {
    capturedResponse.clear();
    capturedResponse.assign(response, response + length);
}

class HandlerTest : public ::testing::Test {
   protected:
    void SetUp() override {
        initParserContext(&parserCtx);
        initHandlerContext(&handlerCtx, mockIspTransfer, mockResponseWriter);

        capturedResponse.clear();
    }

    parser_context_t parserCtx;
    handler_context_t handlerCtx;
};

TEST_F(HandlerTest, HandleGetSyncCommand) {
    parserCtx.state = PARSER_ACCEPTED;
    parserCtx.command = STK500_CMD_GET_SYNC;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(HandlerTest, HandleParserError) {
    parserCtx.state = PARSER_ERROR;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_NO_SYNC};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(HandlerTest, HandleUnknownCommand) {
    parserCtx.state = PARSER_UNKNOWN;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_UNKNOWN};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

}  // namespace
