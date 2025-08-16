#ifndef HANDLER_H
#define HANDLER_H

#include <stdbool.h>
#include <stdint.h>

#include "parser.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ISP書込み関数
 */
typedef uint8_t (*IspTransferFunction)(
    uint8_t cmd1,
    uint8_t cmd2,
    uint8_t cmd3,
    uint8_t cmd4);

/**
 * @brief レスポンス書き込み関数
 */
typedef void (*ResponseWriterFunction)(const uint8_t* response, uint16_t length);

/**
 * @brief パーサコンテキストを渡してコマンドを処理する
 *
 * @param context コンテキスト
 * @param transferFunc ISP書込み関数
 * @param responseWriterFunc レスポンス書き込み関数
 */
void handleCommand(
    parser_context_t* context,
    IspTransferFunction transferFunc,
    ResponseWriterFunction responseWriterFunc);

#ifdef __cplusplus
}
#endif

#endif  // HANDLER_H
