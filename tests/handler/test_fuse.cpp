#include "common.h"

using namespace handler_test;

namespace {

class FuseHandlerTest : public HandlerTestBase {
   protected:
    void SetUp() override {
        HandlerTestBase::SetUp();
        parserCtx.state = PARSER_ACCEPTED;
    }
};

TEST_F(FuseHandlerTest, HandleProgFuseCommand) {
    parserCtx.command = STK500_CMD_PROG_FUSE;
    parserCtx.expectedArgumentsLength = 2;
    parserCtx.receivedArgumentsLength = 2;
    parserCtx.arguments[0] = 0xE2;  // fuse_low
    parserCtx.arguments[1] = 0xDF;  // fuse_high

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(FuseHandlerTest, HandleReadFuseCommand) {
    mockIspTransferReturnValue = 0xE2;
    parserCtx.command = STK500_CMD_READ_FUSE;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0xE2, 0xE2, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(FuseHandlerTest, HandleProgFuseExtCommand) {
    parserCtx.command = STK500_CMD_PROG_FUSE_EXT;
    parserCtx.expectedArgumentsLength = 3;
    parserCtx.receivedArgumentsLength = 3;
    parserCtx.arguments[0] = 0xE2;  // fuse_low
    parserCtx.arguments[1] = 0xDF;  // fuse_high
    parserCtx.arguments[2] = 0xFE;  // fuse_ext

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(FuseHandlerTest, HandleReadFuseExtCommand) {
    mockIspTransferReturnValue = 0xE2;
    parserCtx.command = STK500_CMD_READ_FUSE_EXT;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0xE2, 0xE2, 0xE2, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(FuseHandlerTest, HandleProgLockCommand) {
    parserCtx.command = STK500_CMD_PROG_LOCK;
    parserCtx.expectedArgumentsLength = 1;
    parserCtx.receivedArgumentsLength = 1;
    parserCtx.arguments[0] = 0xFC;  // lock bits

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

TEST_F(FuseHandlerTest, HandleReadLockCommand) {
    mockIspTransferReturnValue = 0xFC;
    parserCtx.command = STK500_CMD_READ_LOCK;
    parserCtx.expectedArgumentsLength = 0;
    parserCtx.receivedArgumentsLength = 0;

    handleCommand(&parserCtx, &handlerCtx);

    std::vector<uint8_t> expectedResponse = {STK500_RESP_IN_SYNC, 0xFC, STK500_RESP_OK};
    ASSERT_EQ(capturedResponse.size(), expectedResponse.size());
    EXPECT_EQ(capturedResponse, expectedResponse);
}

}  // namespace