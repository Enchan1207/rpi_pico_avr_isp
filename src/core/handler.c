#include "core/handler.h"

#include <stddef.h>

#include "core/stk500.h"

// DIされる関数を引数で取り回すと面倒なので、関数ポインタをstatic変数で保持する
static IspTransferFunction transferData = NULL;
static ResponseWriterFunction writeResponse = NULL;

static void handleGetSync(parser_context_t* context) {
    // TODO: 実装
}

static void handleGetSignOn(parser_context_t* context) {
    // TODO: 実装
}

static void handleSetParameter(parser_context_t* context) {
    // TODO: 実装
}

static void handleGetParameter(parser_context_t* context) {
    // TODO: 実装
}

static void handleSetDevice(parser_context_t* context) {
    // TODO: 実装
}

static void handleSetDeviceExt(parser_context_t* context) {
    // TODO: 実装
}

static void handleEnterProgMode(parser_context_t* context) {
    // TODO: 実装
}

static void handleLeaveProgMode(parser_context_t* context) {
    // TODO: 実装
}

static void handleChipErase(parser_context_t* context) {
    // TODO: 実装
}

static void handleCheckAutoInc(parser_context_t* context) {
    // TODO: 実装
}

static void handleLoadAddress(parser_context_t* context) {
    // TODO: 実装
}

static void handleUniversal(parser_context_t* context) {
    // TODO: 実装
}

static void handleUniversalMulti(parser_context_t* context) {
    // TODO: 実装
}

static void handleProgFlash(parser_context_t* context) {
    // TODO: 実装
}

static void handleProgData(parser_context_t* context) {
    // TODO: 実装
}

static void handleProgFuse(parser_context_t* context) {
    // TODO: 実装
}

static void handleProgLock(parser_context_t* context) {
    // TODO: 実装
}

static void handleProgPage(parser_context_t* context) {
    // TODO: 実装
}

static void handleProgFuseExt(parser_context_t* context) {
    // TODO: 実装
}

static void handleReadFlash(parser_context_t* context) {
    // TODO: 実装
}

static void handleReadData(parser_context_t* context) {
    // TODO: 実装
}

static void handleReadFuse(parser_context_t* context) {
    // TODO: 実装
}

static void handleReadLock(parser_context_t* context) {
    // TODO: 実装
}

static void handleReadPage(parser_context_t* context) {
    // TODO: 実装
}

static void handleReadSign(parser_context_t* context) {
    // TODO: 実装
}

static void handleReadOscCal(parser_context_t* context) {
    // TODO: 実装
}

static void handleReadFuseExt(parser_context_t* context) {
    // TODO: 実装
}

static void handleReadOscCalExt(parser_context_t* context) {
    // TODO: 実装
}

static void handleError(parser_context_t* context) {
    if (context->state == PARSER_UNKNOWN) {
        const uint8_t response[] = {STK500_RESP_UNKNOWN};
        writeResponse(response, sizeof(response));
        return;
    }

    // state = PARSER_ERROR
    const uint8_t response[] = {STK500_RESP_NO_SYNC};
    writeResponse(response, sizeof(response));
}

void handleCommand(parser_context_t* context, IspTransferFunction transferFunc, ResponseWriterFunction responseWriterFunc) {
    transferData = transferFunc;
    writeResponse = responseWriterFunc;

    if (context->state != PARSER_ACCEPTED) {
        handleError(context);
        return;
    }

    switch (context->command) {
        case STK500_CMD_GET_SYNC:
            handleGetSync(context);
            break;

        case STK500_CMD_GET_SIGN_ON:
            handleGetSignOn(context);
            break;

        case STK500_CMD_SET_PARAMETER:
            handleSetParameter(context);
            break;

        case STK500_CMD_GET_PARAMETER:
            handleGetParameter(context);
            break;

        case STK500_CMD_SET_DEVICE:
            handleSetDevice(context);
            break;

        case STK500_CMD_SET_DEVICE_EXT:
            handleSetDeviceExt(context);
            break;

        case STK500_CMD_ENTER_PROG_MODE:
            handleEnterProgMode(context);
            break;

        case STK500_CMD_LEAVE_PROG_MODE:
            handleLeaveProgMode(context);
            break;

        case STK500_CMD_CHIP_ERASE:
            handleChipErase(context);
            break;

        case STK500_CMD_CHECK_AUTOINC:
            handleCheckAutoInc(context);
            break;

        case STK500_CMD_LOAD_ADDRESS:
            handleLoadAddress(context);
            break;

        case STK500_CMD_UNIVERSAL:
            handleUniversal(context);
            break;

        case STK500_CMD_UNIVERSAL_MULTI:
            handleUniversalMulti(context);
            break;

        case STK500_CMD_PROG_FLASH:
            handleProgFlash(context);
            break;

        case STK500_CMD_PROG_DATA:
            handleProgData(context);
            break;

        case STK500_CMD_PROG_FUSE:
            handleProgFuse(context);
            break;

        case STK500_CMD_PROG_LOCK:
            handleProgLock(context);
            break;

        case STK500_CMD_PROG_PAGE:
            handleProgPage(context);
            break;

        case STK500_CMD_PROG_FUSE_EXT:
            handleProgFuseExt(context);
            break;

        case STK500_CMD_READ_FLASH:
            handleReadFlash(context);
            break;

        case STK500_CMD_READ_DATA:
            handleReadData(context);
            break;

        case STK500_CMD_READ_FUSE:
            handleReadFuse(context);
            break;

        case STK500_CMD_READ_LOCK:
            handleReadLock(context);
            break;

        case STK500_CMD_READ_PAGE:
            handleReadPage(context);
            break;

        case STK500_CMD_READ_SIGN:
            handleReadSign(context);
            break;

        case STK500_CMD_READ_OSC_CAL:
            handleReadOscCal(context);
            break;

        case STK500_CMD_READ_FUSE_EXT:
            handleReadFuseExt(context);
            break;

        case STK500_CMD_READ_OSC_CAL_EXT:
            handleReadOscCalExt(context);
            break;
    }
}
