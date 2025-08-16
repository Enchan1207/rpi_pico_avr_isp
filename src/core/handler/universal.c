#include "handler_private.h"

void handleUniversal(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t byte1 = parserCtx->arguments[0];
    uint8_t byte2 = parserCtx->arguments[1];
    uint8_t byte3 = parserCtx->arguments[2];
    uint8_t byte4 = parserCtx->arguments[3];

    uint8_t result = handlerCtx->transferFunc(byte1, byte2, byte3, byte4);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, result, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleUniversalMulti(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t numberOfBytes = parserCtx->arguments[0];

    if (numberOfBytes % 4 != 0) {
        const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_FAILED};
        handlerCtx->writeResponse(response, sizeof(response));
        return;
    }

    for (uint8_t i = 0; i < numberOfBytes; i += 4) {
        uint8_t byte1 = parserCtx->arguments[1 + i];
        uint8_t byte2 = parserCtx->arguments[1 + i + 1];
        uint8_t byte3 = parserCtx->arguments[1 + i + 2];
        uint8_t byte4 = parserCtx->arguments[1 + i + 3];

        handlerCtx->transferFunc(byte1, byte2, byte3, byte4);
    }

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}
