#include "handler_test_common.h"

namespace handler_test {

class HandlerErrorTest : public HandlerTestBase {};

TEST_F(HandlerErrorTest, HandleParserError) {
    parserCtx.state = PARSER_ERROR;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_NO_SYNC};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(HandlerErrorTest, HandleUnknownCommand) {
    parserCtx.state = PARSER_UNKNOWN;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_UNKNOWN};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

}  // namespace