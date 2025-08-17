#include "handler_private.h"

void handleProgPage(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // const uint16_t numberOfWords = parserCtx->arguments[0] << 8 | parserCtx->arguments[1];
    // const char memoryType = parserCtx->arguments[2];
    // uint8_t* data = parserCtx->arguments + 3;

    // TODO: ページ書き込み
    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadPage(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    const uint16_t numberOfWords = parserCtx->arguments[0] << 8 | parserCtx->arguments[1];
    // const char memoryType = parserCtx->arguments[2];

    uint8_t response[numberOfWords];
    size_t index = 0;

    response[index++] = STK500_RESP_IN_SYNC;
    for (size_t i = 0; i < numberOfWords; i++) {
        // TODO: フラッシュの各ワードを読み込んで代入
        response[index++] = 0x00;
    }
    response[index++] = STK500_RESP_OK;

    handlerCtx->writeResponse(response, index);
}
