#include "handler_private.h"

void handleSetParameter(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleGetParameter(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleSetDevice(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleSetDeviceExt(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleReadSign(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t signHigh = handlerCtx->transferFunc(0x30, 0x00, 0x00, 0x00);
    uint8_t signMiddle = handlerCtx->transferFunc(0x30, 0x00, 0x01, 0x00);
    uint8_t signLow = handlerCtx->transferFunc(0x30, 0x00, 0x02, 0x00);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, signHigh, signMiddle, signLow, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadOscCal(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t oscCalByte = handlerCtx->transferFunc(0x38, 0x00, 0x00, 0x00);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, oscCalByte, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadOscCalExt(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t address = parserCtx->arguments[0];
    uint8_t oscCalByte = handlerCtx->transferFunc(0x38, 0x00, address, 0x00);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, oscCalByte, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}
