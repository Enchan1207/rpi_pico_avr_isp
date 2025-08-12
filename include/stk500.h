#ifndef STK500_H
#define STK500_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "commands.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief STK500v1プロトコルハンドラーの初期化
 */
void stk500Init(void);

/**
 * @brief STK500v1プロトコルのコマンド処理
 * @return true: 継続、false: 終了
 */
bool stk500ProcessCommands(void);

#ifdef __cplusplus
}
#endif

#endif /* STK500_H */
