#include "handler_private.h"

/// @brief 現在のアドレスが属するページを取得する
/// @param handlerCtx コンテキスト
/// @return アドレスに対応するページ
static uint16_t getCurrentPage(const handler_context_t* handlerCtx) {
    const uint16_t pageSize = handlerCtx->deviceInfo.pageSize;
    const uint16_t currentAddress = handlerCtx->currentAddress;

    if (pageSize == 32) {
        return currentAddress >> 4 << 4;
    }

    if (pageSize == 64) {
        return currentAddress >> 5 << 5;
    }

    if (pageSize == 128) {
        return currentAddress >> 6 << 6;
    }

    if (pageSize == 256) {
        return currentAddress >> 7 << 7;
    }

    // fallback
    return currentAddress;
}

void handleProgPage(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    const size_t numberOfBytes = parserCtx->arguments[0] << 8 | parserCtx->arguments[1];
    uint8_t response[numberOfBytes + 2];

    const char memoryType = parserCtx->arguments[2];
    const uint8_t* data = parserCtx->arguments + 3;

    // NOTE: フラッシュメモリの場合1アドレスに対して2byte書き込むため、ループ回数はデータ数の半分でよい
    const size_t loopCount = memoryType == 'F' ? numberOfBytes >> 1 : numberOfBytes;

    for (size_t i = 0; i < loopCount; i++) {
        const uint16_t base = handlerCtx->currentAddress;

        // if (memoryType == 'E') {
        //     const uint8_t data = handlerCtx->transfer(0xA0, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), 0x00);

        //     handlerCtx->sleep(11);

        //     response[index++] = data;
        // } else {
        //     const uint8_t dataLow = handlerCtx->transfer(0x20, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), 0x00);
        //     const uint8_t dataHigh = handlerCtx->transfer(0x28, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), 0x00);

        //     handlerCtx->sleep(14);

        //     response[index++] = dataLow;
        //     response[index++] = dataHigh;
        // }

        handlerCtx->currentAddress = base + 1;
    }

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

    // NOTE: フラッシュメモリの場合currentAddressがワードアドレスとなるため、ループ回数はデータ数の半分でよい
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
