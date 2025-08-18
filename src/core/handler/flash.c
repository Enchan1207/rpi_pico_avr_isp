#include "handler_private.h"

// FIXME(claude): 現在の実装は不正確です。0x40/0x48はページバッファへの書き込みコマンドであり、
// FIXME(claude): 実際のFlashメモリへの書き込みではありません。正しい実装には以下が必要です：
// FIXME(claude): 1. ページ読み込み (既存ページデータを取得)
// FIXME(claude): 2. 当該ワードを置き換え
// FIXME(claude): 3. ページ書き込み (修正されたページ全体をFlashに書き込み)
// FIXME(claude): page_access.cの実装完了後に修正予定。

void handleProgFlash(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t flashLow = parserCtx->arguments[0];
    uint8_t flashHigh = parserCtx->arguments[1];
    uint16_t address = handlerCtx->currentAddress;

    uint8_t addressHigh = (address >> 8) & 0xFF;
    uint8_t addressLow = address & 0xFF;

    handlerCtx->transfer(0x40, addressHigh, addressLow, flashLow);
    handlerCtx->transfer(0x48, addressHigh, addressLow, flashHigh);

    handlerCtx->sleep(14);

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
