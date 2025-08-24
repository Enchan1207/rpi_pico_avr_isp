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

uint32_t mockSetISPBaudRate(uint32_t baudRate) {
    // テスト環境では要求されたボーレートをそのまま返す
    return baudRate;
}

void HandlerTestBase::SetUp() {
    initParserContext(&parserCtx, argumentsBuffer, sizeof(argumentsBuffer));
    initHandlerContext(&handlerCtx, mockIspTransfer, mockResponseWriter, mockResetControl, mockSleep, mockSetISPBaudRate, responseBuffer, sizeof(responseBuffer));
    capturedResponse.clear();
    mockIspTransferReturnValue = 0x00;
    mockResetState = false;
}

}  // namespace handler_test