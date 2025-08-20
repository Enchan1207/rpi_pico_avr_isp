#include "handler_private.h"

uint16_t getCurrentFlashPage(const handler_context_t* handlerCtx) {
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

uint16_t getCurrentEepromPage(const handler_context_t* handlerCtx) {
    const uint8_t eepromPageSize = handlerCtx->deviceInfo.eepromPageSize;
    const uint16_t currentAddress = handlerCtx->currentAddress;

    if (eepromPageSize == 4) {
        return currentAddress >> 2 << 2;
    }

    if (eepromPageSize == 8) {
        return currentAddress >> 3 << 3;
    }

    if (eepromPageSize == 16) {
        return currentAddress >> 4 << 4;
    }

    if (eepromPageSize == 32) {
        return currentAddress >> 5 << 5;
    }

    // fallback
    return currentAddress;
}

void handleProgPage(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    const size_t numberOfBytes = parserCtx->arguments[0] << 8 | parserCtx->arguments[1];
    const char memoryType = parserCtx->arguments[2];
    const uint8_t* data = parserCtx->arguments + 3;

    // NOTE: フラッシュメモリの場合1アドレスに対して2byte書き込む
    const size_t step = memoryType == 'F' ? 2 : 1;
    for (size_t i = 0; i < numberOfBytes; i += step) {
        const uint16_t base = handlerCtx->currentAddress;
        const uint16_t currentPage = memoryType == 'F' ? getCurrentFlashPage(handlerCtx) : getCurrentEepromPage(handlerCtx);

        // 1. バッファに書き込む
        if (memoryType == 'F') {
            handlerCtx->transfer(0x40, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), data[i]);
            handlerCtx->transfer(0x48, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), data[i + 1]);
        } else {
            handlerCtx->transfer(0xC0, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), data[i]);
        }

        // 2. アドレスを進める
        handlerCtx->currentAddress = base + 1;

        // 3. ページが変わった場合は、今のページを書き込む
        const uint16_t newPage = memoryType == 'F' ? getCurrentFlashPage(handlerCtx) : getCurrentEepromPage(handlerCtx);
        if (currentPage == newPage) {
            continue;
        }

        if (memoryType == 'F') {
            handlerCtx->transfer(0x4C, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), 0x00);
            waitForTargetReady(handlerCtx, 15);
        } else {
            handlerCtx->transfer(0xC2, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), 0x00);
            waitForTargetReady(handlerCtx, 10);
        }
    }

    // 最後のページを書き込む
    const uint16_t currentAddress = handlerCtx->currentAddress;
    if (memoryType == 'F') {
        handlerCtx->transfer(0x4C, (uint8_t)(currentAddress >> 8), (uint8_t)(currentAddress & 0xFF), 0x00);
        waitForTargetReady(handlerCtx, 15);
    } else {
        handlerCtx->transfer(0xC2, (uint8_t)(currentAddress >> 8), (uint8_t)(currentAddress & 0xFF), 0x00);
        waitForTargetReady(handlerCtx, 10);
    }

    handlerCtx->writeResponse((uint8_t[]){STK500_RESP_IN_SYNC, STK500_RESP_OK}, 2);
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

    size_t index = 0;
    response[index++] = STK500_RESP_IN_SYNC;

    // NOTE: フラッシュメモリの場合は1アドレス1ワードとなるため、2byteずつ進む
    const size_t step = memoryType == 'F' ? 2 : 1;
    for (size_t i = 0; i < numberOfBytes; i += step) {
        const uint16_t base = handlerCtx->currentAddress;

        if (memoryType == 'E') {
            const uint8_t data = handlerCtx->transfer(0xA0, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), 0x00);

            waitForTargetReady(handlerCtx, 10);

            response[index++] = data;
        } else {
            const uint8_t dataLow = handlerCtx->transfer(0x20, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), 0x00);
            const uint8_t dataHigh = handlerCtx->transfer(0x28, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), 0x00);

            waitForTargetReady(handlerCtx, 15);

            response[index++] = dataLow;
            response[index++] = dataHigh;
        }

        handlerCtx->currentAddress = base + 1;
    }

    response[index++] = STK500_RESP_OK;

    handlerCtx->writeResponse(response, index);
}
