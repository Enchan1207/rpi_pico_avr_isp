#include "common.h"

namespace handler_test {

uint8_t mockIspTransferReturnValue = 0x00;

uint8_t mockIspTransfer(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3, uint8_t cmd4) {
    return mockIspTransferReturnValue;
}

std::vector<uint8_t> capturedResponse;
void mockResponseWriter(const uint8_t* response, size_t length) {
    capturedResponse.clear();
    capturedResponse.assign(response, response + length);
}

bool mockResetState = false;
void mockResetControl(bool state) {
    mockResetState = state;
}

void mockSleep(uint32_t milliseconds) {
    // テスト環境では実際の待機は不要
}

void HandlerTestBase::SetUp() {
    initParserContext(&parserCtx);
    initHandlerContext(&handlerCtx, mockIspTransfer, mockResponseWriter, mockResetControl, mockSleep);
    capturedResponse.clear();
    mockIspTransferReturnValue = 0x00;
    mockResetState = false;
}

}  // namespace handler_test