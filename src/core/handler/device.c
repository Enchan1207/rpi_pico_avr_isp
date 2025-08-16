#include "core/stk500_parameters.h"
#include "handler_private.h"

void handleSetParameter(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t parameter = parserCtx->arguments[0];

    switch (parameter) {
        case PARM_STK_HW_VER:
        case PARM_STK_SW_MAJOR:
        case PARM_STK_SW_MINOR:
        case PARM_STK_BUFSIZEL:
        case PARM_STK_BUFSIZEH:
        case PARAM_STK500_TOPCARD_DETECT: {
            const uint8_t failedResponse[] = {STK500_RESP_IN_SYNC, parameter, STK500_RESP_FAILED};
            handlerCtx->writeResponse(failedResponse, sizeof(failedResponse));
            break;
        }

        case PARM_STK_LEDS:
        case PARM_STK_VTARGET:
        case PARM_STK_VADJUST:
        case PARM_STK_OSC_PSCALE:
        case PARM_STK_OSC_CMATCH:
        case PARM_STK_SCK_DURATION: {
            const uint8_t okResponse[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
            handlerCtx->writeResponse(okResponse, sizeof(okResponse));
            break;
        }

        default: {
            const uint8_t unknownResponse[] = {STK500_RESP_IN_SYNC, parameter, STK500_RESP_FAILED};
            handlerCtx->writeResponse(unknownResponse, sizeof(unknownResponse));
            break;
        }
    }
}

void handleGetParameter(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t parameter = parserCtx->arguments[0];
    uint8_t value;
    uint8_t status;

    switch (parameter) {
        case PARM_STK_HW_VER:
            value = STK500_HW_VERSION;
            status = STK500_RESP_OK;
            break;

        case PARM_STK_SW_MAJOR:
            value = STK500_SW_MAJOR;
            status = STK500_RESP_OK;
            break;

        case PARM_STK_SW_MINOR:
            value = STK500_SW_MINOR;
            status = STK500_RESP_OK;
            break;

        case PARAM_STK500_TOPCARD_DETECT:
            value = 0x03;
            status = STK500_RESP_OK;
            break;

        case PARM_STK_BUFSIZEL:
        case PARM_STK_BUFSIZEH:
        case PARM_STK_LEDS:
        case PARM_STK_VTARGET:
        case PARM_STK_VADJUST:
        case PARM_STK_OSC_PSCALE:
        case PARM_STK_OSC_CMATCH:
        case PARM_STK_SCK_DURATION:
            value = 0x00;
            status = STK500_RESP_OK;
            break;

        default:
            value = parameter;
            status = STK500_RESP_FAILED;
            break;
    }

    const uint8_t response[] = {STK500_RESP_IN_SYNC, value, status};
    handlerCtx->writeResponse(response, sizeof(response));
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
