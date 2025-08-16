#include "core/handler.h"

#include <stddef.h>

#include "core/stk500.h"
#include "handler_private.h"

void handleGetSync(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleGetSignOn(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleSetParameter(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleGetParameter(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleSetDevice(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleSetDeviceExt(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleEnterProgMode(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleLeaveProgMode(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleChipErase(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleCheckAutoInc(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleLoadAddress(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleUniversal(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleUniversalMulti(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleProgFlash(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleProgData(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleProgFuse(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleProgLock(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleProgPage(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleProgFuseExt(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleReadFlash(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleReadData(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleReadFuse(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleReadLock(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleReadPage(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleReadSign(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleReadOscCal(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleReadFuseExt(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleReadOscCalExt(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // TODO: 実装
}

void handleError(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    if (parserCtx->state == PARSER_UNKNOWN) {
        const uint8_t response[] = {STK500_RESP_UNKNOWN};
        handlerCtx->writeResponse(response, sizeof(response));
        return;
    }

    // state = PARSER_ERROR
    const uint8_t response[] = {STK500_RESP_NO_SYNC};
    handlerCtx->writeResponse(response, sizeof(response));
}

void initHandlerContext(handler_context_t* context, IspTransferFunction transferFunc, ResponseWriterFunction responseWriterFunc) {
    context->transferFunc = transferFunc;
    context->writeResponse = responseWriterFunc;
}

void handleCommand(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    if (parserCtx->state != PARSER_ACCEPTED) {
        handleError(parserCtx, handlerCtx);
        return;
    }

    switch (parserCtx->command) {
        case STK500_CMD_GET_SYNC:
            handleGetSync(parserCtx, handlerCtx);
            break;

        case STK500_CMD_GET_SIGN_ON:
            handleGetSignOn(parserCtx, handlerCtx);
            break;

        case STK500_CMD_SET_PARAMETER:
            handleSetParameter(parserCtx, handlerCtx);
            break;

        case STK500_CMD_GET_PARAMETER:
            handleGetParameter(parserCtx, handlerCtx);
            break;

        case STK500_CMD_SET_DEVICE:
            handleSetDevice(parserCtx, handlerCtx);
            break;

        case STK500_CMD_SET_DEVICE_EXT:
            handleSetDeviceExt(parserCtx, handlerCtx);
            break;

        case STK500_CMD_ENTER_PROG_MODE:
            handleEnterProgMode(parserCtx, handlerCtx);
            break;

        case STK500_CMD_LEAVE_PROG_MODE:
            handleLeaveProgMode(parserCtx, handlerCtx);
            break;

        case STK500_CMD_CHIP_ERASE:
            handleChipErase(parserCtx, handlerCtx);
            break;

        case STK500_CMD_CHECK_AUTOINC:
            handleCheckAutoInc(parserCtx, handlerCtx);
            break;

        case STK500_CMD_LOAD_ADDRESS:
            handleLoadAddress(parserCtx, handlerCtx);
            break;

        case STK500_CMD_UNIVERSAL:
            handleUniversal(parserCtx, handlerCtx);
            break;

        case STK500_CMD_UNIVERSAL_MULTI:
            handleUniversalMulti(parserCtx, handlerCtx);
            break;

        case STK500_CMD_PROG_FLASH:
            handleProgFlash(parserCtx, handlerCtx);
            break;

        case STK500_CMD_PROG_DATA:
            handleProgData(parserCtx, handlerCtx);
            break;

        case STK500_CMD_PROG_FUSE:
            handleProgFuse(parserCtx, handlerCtx);
            break;

        case STK500_CMD_PROG_LOCK:
            handleProgLock(parserCtx, handlerCtx);
            break;

        case STK500_CMD_PROG_PAGE:
            handleProgPage(parserCtx, handlerCtx);
            break;

        case STK500_CMD_PROG_FUSE_EXT:
            handleProgFuseExt(parserCtx, handlerCtx);
            break;

        case STK500_CMD_READ_FLASH:
            handleReadFlash(parserCtx, handlerCtx);
            break;

        case STK500_CMD_READ_DATA:
            handleReadData(parserCtx, handlerCtx);
            break;

        case STK500_CMD_READ_FUSE:
            handleReadFuse(parserCtx, handlerCtx);
            break;

        case STK500_CMD_READ_LOCK:
            handleReadLock(parserCtx, handlerCtx);
            break;

        case STK500_CMD_READ_PAGE:
            handleReadPage(parserCtx, handlerCtx);
            break;

        case STK500_CMD_READ_SIGN:
            handleReadSign(parserCtx, handlerCtx);
            break;

        case STK500_CMD_READ_OSC_CAL:
            handleReadOscCal(parserCtx, handlerCtx);
            break;

        case STK500_CMD_READ_FUSE_EXT:
            handleReadFuseExt(parserCtx, handlerCtx);
            break;

        case STK500_CMD_READ_OSC_CAL_EXT:
            handleReadOscCalExt(parserCtx, handlerCtx);
            break;

        default:
            // 何もしない (パーサコンテキストがacceptedなら、ここには落ちないはず)
            break;
    }
}
