#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdint.h>

#include "stk500.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 与えられたコマンドの引数長を返す
 *
 * @param command STK500コマンド
 * @param length 引数長を格納するポインタ
 * @return bool コマンドが有効かどうか (未定義の場合falseが返ります)
 *
 * @note 引数内にデータ全体長の情報が含まれる可変長のコマンドについては、
 *       最低限必要な引数の長さを返します。
 */
bool getCommandArgumentsLength(Stk500Command command, uint8_t* length);

#ifdef __cplusplus
}
#endif

#endif  // PARSER_H
