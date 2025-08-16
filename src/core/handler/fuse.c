#include "handler_private.h"

void handleProgFuse(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t fuse_low = parserCtx->arguments[0];
    uint8_t fuse_high = parserCtx->arguments[1];

    handlerCtx->transferFunc(0xAC, 0xA0, 0x00, fuse_low);
    handlerCtx->transferFunc(0xAC, 0xA8, 0x00, fuse_high);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadFuse(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t fuse_low = handlerCtx->transferFunc(0x50, 0x00, 0x00, 0x00);
    uint8_t fuse_high = handlerCtx->transferFunc(0x58, 0x08, 0x00, 0x00);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, fuse_low, fuse_high, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleProgFuseExt(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t fuse_low = parserCtx->arguments[0];
    uint8_t fuse_high = parserCtx->arguments[1];
    uint8_t fuse_ext = parserCtx->arguments[2];

    handlerCtx->transferFunc(0xAC, 0xA0, 0x00, fuse_low);
    handlerCtx->transferFunc(0xAC, 0xA8, 0x00, fuse_high);
    handlerCtx->transferFunc(0xAC, 0xA4, 0x00, fuse_ext);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadFuseExt(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t fuse_low = handlerCtx->transferFunc(0x50, 0x00, 0x00, 0x00);
    uint8_t fuse_high = handlerCtx->transferFunc(0x58, 0x08, 0x00, 0x00);
    uint8_t fuse_ext = handlerCtx->transferFunc(0x50, 0x08, 0x00, 0x00);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, fuse_low, fuse_high, fuse_ext, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleProgLock(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t lock = parserCtx->arguments[0];

    handlerCtx->transferFunc(0xAC, 0xE0, 0x00, lock);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadLock(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t lock = handlerCtx->transferFunc(0x58, 0x00, 0x00, 0x00);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, lock, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}
