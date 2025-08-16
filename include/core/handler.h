#ifndef HANDLER_H
#define HANDLER_H

#include <stdbool.h>
#include <stddef.h>
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
typedef void (*ResponseWriterFunction)(const uint8_t* response, size_t length);

/**
 * @brief ハンドラコンテキスト
 */
typedef struct {
    /// @brief ISP書込み関数
    IspTransferFunction transferFunc;

    /// @brief レスポンス書き込み関数
    ResponseWriterFunction writeResponse;
} handler_context_t;

/**
 * @brief ハンドラコンテキストを初期化する
 *
 * @param context ハンドラコンテキスト
 * @param transferFunc ISP書込み関数
 * @param responseWriterFunc レスポンス書き込み関数
 */
void initHandlerContext(
    handler_context_t* context,
    IspTransferFunction transferFunc,
    ResponseWriterFunction responseWriterFunc);

/**
 * @brief パーサコンテキストとハンドラコンテキストを渡してコマンドを処理する
 *
 * @param parserCtx パーサコンテキスト
 * @param handlerCtx ハンドラコンテキスト
 */
void handleCommand(
    parser_context_t* parserCtx,
    handler_context_t* handlerCtx);

#ifdef __cplusplus
}
#endif

#endif  // HANDLER_H
