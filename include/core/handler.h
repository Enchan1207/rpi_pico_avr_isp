#ifndef HANDLER_H
#define HANDLER_H

#include <stdbool.h>
#include <stdint.h>

#include "parser.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief データ転送関数
 */
typedef uint8_t (*DataTransferFunction)(
    uint8_t cmd1,
    uint8_t cmd2,
    uint8_t cmd3,
    uint8_t cmd4);

/**
 * @brief パーサコンテキストを渡してコマンドを処理する
 *
 * @param context コンテキスト
 * @param transferFunc データ転送関数
 */
void handleCommand(parser_context_t* context, DataTransferFunction transferFunc);

#ifdef __cplusplus
}
#endif

#endif  // HANDLER_H
