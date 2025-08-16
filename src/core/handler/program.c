#include "handler_private.h"

void handleEnterProgMode(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleLeaveProgMode(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleChipErase(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleCheckAutoInc(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // NOTE: このコマンドは常に成功する
    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleLoadAddress(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint16_t address = (parserCtx->arguments[0] << 8) | parserCtx->arguments[1];
    handlerCtx->currentAddress = address;

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}
