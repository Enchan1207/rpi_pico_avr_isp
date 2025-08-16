#include "handler_private.h"

void handleProgData(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t data = parserCtx->arguments[0];
    uint16_t address = handlerCtx->currentAddress;

    uint8_t addressHigh = (address >> 8) & 0xFF;
    uint8_t addressLow = address & 0xFF;

    handlerCtx->transferFunc(0xC0, addressHigh, addressLow, data);
    
    handlerCtx->sleepFunc(11);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadData(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint16_t address = handlerCtx->currentAddress;

    uint8_t addressHigh = (address >> 8) & 0xFF;
    uint8_t addressLow = address & 0xFF;

    uint8_t data = handlerCtx->transferFunc(0xA0, addressHigh, addressLow, 0x00);

    const uint8_t response[] = {STK500_RESP_IN_SYNC, data, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}
