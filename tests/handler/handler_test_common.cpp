#include "handler_test_common.h"

namespace handler_test {

uint8_t mockIspTransfer(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3, uint8_t cmd4) {
    return 0x00;
}

std::vector<uint8_t> capturedResponse;
void mockResponseWriter(const uint8_t* response, size_t length) {
    capturedResponse.clear();
    capturedResponse.assign(response, response + length);
}

void HandlerTestBase::SetUp() {
    initParserContext(&parserCtx);
    initHandlerContext(&handlerCtx, mockIspTransfer, mockResponseWriter);
    capturedResponse.clear();
}

}  // namespace handler_test