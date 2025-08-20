#include "handler_private.h"

void handleEnterProgMode(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // NOTE: リセット処理後に20ms以上待機
    // NOTE: cf. AVR910 3. Programming Protocol
    handlerCtx->resetControl(false);
    handlerCtx->sleep(20);

    uint8_t result = handlerCtx->transfer(0xAC, 0x53, 0x00, 0x00);

    if (result == 0x53) {
        const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
        handlerCtx->writeResponse(response, sizeof(response));
    } else {
        const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_NO_DEVICE};
        handlerCtx->writeResponse(response, sizeof(response));
    }
}

void handleLeaveProgMode(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    handlerCtx->resetControl(true);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleChipErase(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    handlerCtx->transfer(0xAC, 0x80, 0x00, 0x00);
    handlerCtx->sleep(20);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleCheckAutoInc(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // NOTE: このコマンドは常に成功する
    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleLoadAddress(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint16_t address = parserCtx->arguments[0] | (parserCtx->arguments[1] << 8);
    handlerCtx->currentAddress = address;

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}
