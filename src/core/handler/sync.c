#include <string.h>

#include "handler_private.h"

void handleGetSync(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleGetSignOn(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    const uint8_t signOnMessage[] = "AVR STK";

    uint8_t response[sizeof(signOnMessage) + 1] = {0};
    response[0] = STK500_RESP_IN_SYNC;
    response[sizeof(response) - 1] = STK500_RESP_OK;
    memcpy(response + 1, signOnMessage, sizeof(signOnMessage) - 1);

    handlerCtx->writeResponse(response, sizeof(response));
}
