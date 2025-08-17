#include "handler_private.h"

void handleProgPage(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // const uint16_t numberOfData = parserCtx->arguments[0] << 8 | parserCtx->arguments[1];
    // const char memoryType = parserCtx->arguments[2];
    // uint8_t* data = parserCtx->arguments + 3;

    // TODO: ページ書き込み
    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadPage(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    const size_t numberOfBytes = parserCtx->arguments[0] << 8 | parserCtx->arguments[1];
    uint8_t response[numberOfBytes + 2];

    const char memoryType = parserCtx->arguments[2];

    // 不明なメモリタイプ
    if (memoryType != 'E' && memoryType != 'F') {
        handlerCtx->writeResponse((uint8_t[]){STK500_RESP_NO_SYNC}, 1);
        return;
    }

    // NOTE: フラッシュメモリの場合、currentAddressがワードアドレスとなるため、
    // NOTE: ループ回数は指定されたデータ数の半分でよい
    const size_t loopCount = memoryType == 'F' ? numberOfBytes >> 1 : numberOfBytes;
    size_t index = 0;
    response[index++] = STK500_RESP_IN_SYNC;

    for (size_t i = 0; i < loopCount; i++) {
        const uint16_t base = handlerCtx->currentAddress;

        if (memoryType == 'E') {
            const uint8_t data = handlerCtx->transfer(0xA0, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), 0x00);

            handlerCtx->sleep(11);

            response[index++] = data;
        } else {
            const uint8_t dataLow = handlerCtx->transfer(0x20, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), 0x00);
            const uint8_t dataHigh = handlerCtx->transfer(0x28, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), 0x00);

            handlerCtx->sleep(14);

            response[index++] = dataLow;
            response[index++] = dataHigh;
        }

        handlerCtx->currentAddress = base + 1;
    }

    response[index++] = STK500_RESP_OK;

    handlerCtx->writeResponse(response, index);
}
