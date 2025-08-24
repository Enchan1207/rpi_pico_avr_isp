#include "core/handler.h"

#include "core/stk500.h"
#include "handler_private.h"

void handleError(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    if (parserCtx->state == PARSER_UNKNOWN) {
        const uint8_t response[] = {STK500_RESP_UNKNOWN};
        handlerCtx->writeResponse(response, sizeof(response));
        return;
    }

    // state = PARSER_ERROR
    const uint8_t response[] = {STK500_RESP_NO_SYNC};
    handlerCtx->writeResponse(response, sizeof(response));
}

void initHandlerContext(handler_context_t* context, IspTransferFunction transferFunc, ResponseWriterFunction responseWriterFunc, ResetControlFunction resetControlFunc, SleepFunction sleepFunc, SetISPBaudRateFunction setISPBaudRateFunc, uint8_t* responseBuffer, uint16_t responseBufferSize) {
    context->transfer = transferFunc;
    context->writeResponse = responseWriterFunc;
    context->resetControl = resetControlFunc;
    context->sleep = sleepFunc;
    context->setISPBaudRate = setISPBaudRateFunc;
    context->currentAddress = 0;
    context->responseBuffer = responseBuffer;
    context->responseBufferSize = responseBufferSize;
    
    context->deviceInfo.deviceCode = 0;
    context->deviceInfo.lockBytesLength = 0;
    context->deviceInfo.fuseBytesLength = 0;
    context->deviceInfo.pageSize = 0;
    context->deviceInfo.eepromSize = 0;
    context->deviceInfo.flashSize = 0;
    context->deviceInfo.commandSize = 0;
    context->deviceInfo.eepromPageSize = 0;
    context->deviceInfo.sckDuration = 0x00;
}

void handleCommand(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
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
            handleError(parserCtx, handlerCtx);
            break;
    }
}