#include "handler_private.h"

void handleProgFuse(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t fuseLow = parserCtx->arguments[0];
    uint8_t fuseHigh = parserCtx->arguments[1];

    log("PROGFUSE: high:%02X low:%02X", fuseHigh, fuseLow);

    handlerCtx->transfer(0xAC, 0xA0, 0x00, fuseLow);
    handlerCtx->transfer(0xAC, 0xA8, 0x00, fuseHigh);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadFuse(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t fuseLow = handlerCtx->transfer(0x50, 0x00, 0x00, 0x00);
    uint8_t fuseHigh = handlerCtx->transfer(0x58, 0x08, 0x00, 0x00);

    log("READFUSE: high:%02X low:%02X", fuseHigh, fuseLow);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, fuseLow, fuseHigh, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleProgFuseExt(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t fuseLow = parserCtx->arguments[0];
    uint8_t fuseHigh = parserCtx->arguments[1];
    uint8_t fuseExt = parserCtx->arguments[2];

    log("PROGFUSE_EXT: ext:%02X high:%02X low: %02X", fuseExt, fuseHigh, fuseLow);

    handlerCtx->transfer(0xAC, 0xA0, 0x00, fuseLow);
    handlerCtx->transfer(0xAC, 0xA8, 0x00, fuseHigh);
    handlerCtx->transfer(0xAC, 0xA4, 0x00, fuseExt);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadFuseExt(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t fuseLow = handlerCtx->transfer(0x50, 0x00, 0x00, 0x00);
    uint8_t fuseHigh = handlerCtx->transfer(0x58, 0x08, 0x00, 0x00);
    uint8_t fuseExt = handlerCtx->transfer(0x50, 0x08, 0x00, 0x00);

    log("READFUSE_EXT: ext: %02X high:%02X low:%02X", fuseExt, fuseHigh, fuseLow);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, fuseLow, fuseHigh, fuseExt, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleProgLock(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t lock = parserCtx->arguments[0];

    log("PROGLOCK: %02X", lock);

    handlerCtx->transfer(0xAC, 0xE0, 0x00, lock);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadLock(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t lock = handlerCtx->transfer(0x58, 0x00, 0x00, 0x00);

    log("READLOCK: %02X", lock);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, lock, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}
