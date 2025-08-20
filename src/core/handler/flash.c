#include "handler_private.h"

void handleProgFlash(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t flashLow = parserCtx->arguments[0];
    uint8_t flashHigh = parserCtx->arguments[1];
    uint16_t targetAddress = handlerCtx->currentAddress;

    uint16_t pageStart = getCurrentFlashPage(handlerCtx);
    uint16_t pageSize = handlerCtx->deviceInfo.pageSize;
    uint16_t pageEnd = pageStart + pageSize - 1;

    // ページごとバッファに起こす
    for (uint16_t addr = pageStart; addr <= pageEnd; addr++) {
        uint8_t addressHigh = (addr >> 8) & 0xFF;
        uint8_t addressLow = addr & 0xFF;

        uint8_t dataLow, dataHigh;

        // 書き換え対象なら引数、そうでなければ既存データを読み出してバッファに書き込む
        if (addr == targetAddress) {
            dataLow = flashLow;
            dataHigh = flashHigh;
        } else {
            dataLow = handlerCtx->transfer(0x20, addressHigh, addressLow, 0x00);
            dataHigh = handlerCtx->transfer(0x28, addressHigh, addressLow, 0x00);
        }

        handlerCtx->transfer(0x40, addressHigh, addressLow, dataLow);
        handlerCtx->transfer(0x48, addressHigh, addressLow, dataHigh);
    }

    // ページごと書き戻す
    uint8_t pageStartHigh = (pageStart >> 8) & 0xFF;
    uint8_t pageStartLow = pageStart & 0xFF;
    handlerCtx->transfer(0x4C, pageStartHigh, pageStartLow, 0x00);

    waitForTargetReady(handlerCtx, 15);

    handlerCtx->currentAddress++;

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadFlash(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint16_t address = handlerCtx->currentAddress;

    uint8_t addressHigh = (address >> 8) & 0xFF;
    uint8_t addressLow = address & 0xFF;

    uint8_t flashLow = handlerCtx->transfer(0x20, addressHigh, addressLow, 0x00);
    uint8_t flashHigh = handlerCtx->transfer(0x28, addressHigh, addressLow, 0x00);

    handlerCtx->currentAddress++;

    const uint8_t response[] = {STK500_RESP_IN_SYNC, flashLow, flashHigh, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}
