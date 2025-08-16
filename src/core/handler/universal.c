#include "handler_private.h"

void handleUniversal(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // 引数が4バイトであることを確認
    if (parserCtx->receivedArgumentsLength != 4) {
        const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_FAILED};
        handlerCtx->writeResponse(response, sizeof(response));
        return;
    }

    // ISPで4バイトのコマンドを送信し、最後のバイトの応答を取得
    uint8_t byte1 = parserCtx->arguments[0];
    uint8_t byte2 = parserCtx->arguments[1];
    uint8_t byte3 = parserCtx->arguments[2];
    uint8_t byte4 = parserCtx->arguments[3];

    uint8_t result = handlerCtx->transferFunc(byte1, byte2, byte3, byte4);

    // レスポンスを送信: IN_SYNC + 応答データ + OK
    const uint8_t response[] = {STK500_RESP_IN_SYNC, result, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}

void handleUniversalMulti(parser_context_t* parserCtx, handler_context_t* handlerCtx) {
    // 最低1バイト（データ長）が必要
    if (parserCtx->receivedArgumentsLength < 1) {
        const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_FAILED};
        handlerCtx->writeResponse(response, sizeof(response));
        return;
    }

    uint8_t numberOfBytes = parserCtx->arguments[0];

    // データ長の整合性を確認
    if (parserCtx->receivedArgumentsLength != (numberOfBytes + 1)) {
        const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_FAILED};
        handlerCtx->writeResponse(response, sizeof(response));
        return;
    }

    // データは4バイト単位でISPコマンドとして送信する必要がある
    if (numberOfBytes % 4 != 0) {
        const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_FAILED};
        handlerCtx->writeResponse(response, sizeof(response));
        return;
    }

    // 各4バイトをISPコマンドとして送信
    for (uint8_t i = 0; i < numberOfBytes; i += 4) {
        uint8_t byte1 = parserCtx->arguments[1 + i];
        uint8_t byte2 = parserCtx->arguments[1 + i + 1];
        uint8_t byte3 = parserCtx->arguments[1 + i + 2];
        uint8_t byte4 = parserCtx->arguments[1 + i + 3];

        // ISPコマンドを送信（応答は無視）
        handlerCtx->transferFunc(byte1, byte2, byte3, byte4);
    }

    // 成功レスポンスを送信
    const uint8_t response[] = {STK500_RESP_IN_SYNC, STK500_RESP_OK};
    handlerCtx->writeResponse(response, sizeof(response));
}
