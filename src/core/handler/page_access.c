#include "handler_private.h"

bool waitForTargetReady(const handler_context_t* handlerCtx, uint8_t retryCount) {
    uint8_t isBusy = 1;
    while (retryCount-- && isBusy) {
        isBusy = handlerCtx->transfer(0xF0, 0x00, 0x00, 0x00);
        handlerCtx->sleep(1);
    }

    return isBusy == 0;
}

uint16_t getCurrentPage(const handler_context_t* handlerCtx, char memoryType) {
    if (memoryType == 'F') {
        return getCurrentFlashPage(handlerCtx);
    } else {
        return getCurrentEepromPage(handlerCtx);
    }
}

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

/**
 * @brief ハンドラが今見ているページをコミットする
 *
 * @param handlerCtx コンテキスト
 * @param memoryType メモリ種別(F=フラッシュメモリ, E=EEPROM)
 * @param pageAddress 書き込むページのアドレス
 */
static void commitPage(handler_context_t* handlerCtx, char memoryType, uint16_t pageAddress) {
    log("committing page 0x%04X", pageAddress);

    if (memoryType == 'F') {
        handlerCtx->transfer(0x4C, (uint8_t)(pageAddress >> 8), (uint8_t)(pageAddress & 0xFF), 0x00);
        waitForTargetReady(handlerCtx, 15);
    } else {
        handlerCtx->transfer(0xC2, (uint8_t)(pageAddress >> 8), (uint8_t)(pageAddress & 0xFF), 0x00);
        waitForTargetReady(handlerCtx, 10);
    }
}

void handleProgPage(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    const size_t numberOfBytes = parserCtx->arguments[0] << 8 | parserCtx->arguments[1];
    const char memoryType = parserCtx->arguments[2];
    const uint8_t* data = parserCtx->arguments + 3;

    log("PROGPAGE: %d bytes from %04X to %c", numberOfBytes, handlerCtx->currentAddress, memoryType);

    // NOTE: フラッシュメモリの場合1アドレスに対して2byte書き込む
    const size_t step = memoryType == 'F' ? 2 : 1;
    bool hasUnCommitedPage = false;
    for (size_t i = 0; i < numberOfBytes; i += step) {
        const uint16_t base = handlerCtx->currentAddress;
        // バッファに書き込む
        if (memoryType == 'F') {
            handlerCtx->transfer(0x40, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), data[i]);
            handlerCtx->transfer(0x48, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), data[i + 1]);
        } else {
            handlerCtx->transfer(0xC1, (uint8_t)(base >> 8), (uint8_t)(base & 0xFF), data[i]);
        }

        // アドレスを進め、ページ境界を跨ぐならコミットする
        const uint16_t currentPage = getCurrentPage(handlerCtx, memoryType);
        handlerCtx->currentAddress = base + 1;

        const uint16_t nextPage = getCurrentPage(handlerCtx, memoryType);
        if (currentPage == nextPage) {
            hasUnCommitedPage = true;
            continue;
        }

        log("committing page 0x%04X", currentPage);
        commitPage(handlerCtx, memoryType, currentPage);
        hasUnCommitedPage = false;
    }

    // 必要なら最後のページをコミットする
    if (hasUnCommitedPage) {
        const uint16_t currentPage = getCurrentPage(handlerCtx, memoryType);
        commitPage(handlerCtx, memoryType, currentPage);
    }

    handlerCtx->writeResponse((uint8_t[]){STK500_RESP_IN_SYNC, STK500_RESP_OK}, 2);
}

void handleReadPage(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    const size_t numberOfBytes = parserCtx->arguments[0] << 8 | parserCtx->arguments[1];
    const char memoryType = parserCtx->arguments[2];

    if (handlerCtx->responseBuffer == NULL || handlerCtx->responseBufferSize < numberOfBytes + 2) {
        handlerCtx->writeResponse((uint8_t[]){STK500_RESP_NO_SYNC}, 1);
        return;
    }

    uint8_t* response = handlerCtx->responseBuffer;

    log("READPAGE: %d bytes from %04X to %c", numberOfBytes, handlerCtx->currentAddress, memoryType);

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
