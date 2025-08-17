#include "handler_private.h"

void handleProgFlash(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint8_t flashLow = parserCtx->arguments[0];
    uint8_t flashHigh = parserCtx->arguments[1];
    uint16_t address = handlerCtx->currentAddress;

    uint8_t addressHigh = (address >> 8) & 0xFF;
    uint8_t addressLow = address & 0xFF;

    handlerCtx->transfer(0x40, addressHigh, addressLow, flashLow);
    handlerCtx->transfer(0x48, addressHigh, addressLow, flashHigh);

    handlerCtx->sleep(14);

    handlerCtx->currentAddress++;

    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleReadFlash(const parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    uint16_t address = handlerCtx->currentAddress;

    uint8_t addressHigh = (address >> 8) & 0xFF;
    uint8_t addressLow = address & 0xFF;

    uint8_t flashLow = handlerCtx->transfer(0x20, addressHigh, addressLow, 0x00);
    uint8_t flashHigh = handlerCtx->transfer(0x28, addressHigh, addressLow, 0x00);

    handlerCtx->currentAddress++;

    const uint8_t response[] = {STK500_RESP_IN_SYNC, flashLow, flashHigh, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}
