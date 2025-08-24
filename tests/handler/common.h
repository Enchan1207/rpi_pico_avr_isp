#ifndef HANDLER_TEST_COMMON_H
#define HANDLER_TEST_COMMON_H

#include <gtest/gtest.h>
#include <vector>
#include <cstring>

#include "core/handler.h"
#include "core/parser.h"
#include "core/stk500.h"

namespace handler_test {

extern uint8_t mockIspTransferReturnValue;
uint8_t mockIspTransfer(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3, uint8_t cmd4);

extern std::vector<uint8_t> capturedResponse;
void mockResponseWriter(const uint8_t* response, size_t length);

extern bool mockResetState;
void mockResetControl(bool state);

void mockSleep(uint32_t milliseconds);
uint32_t mockSetISPBaudRate(uint32_t baudRate);

class HandlerTestBase : public ::testing::Test {
protected:
    void SetUp() override;

    parser_context_t parserCtx;
    handler_context_t handlerCtx;
    uint8_t argumentsBuffer[259];
    uint8_t responseBuffer[258];
};

}  // namespace handler_test

#endif  // HANDLER_TEST_COMMON_H