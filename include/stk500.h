#ifndef STK500_H
#define STK500_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief STK500v1プロトコルハンドラーの初期化
 */
void stk500_init(void);

/**
 * @brief STK500v1プロトコルのコマンド処理
 * @return true: 継続、false: 終了
 */
bool stk500_process_commands(void);

/**
 * @brief USB CDC経由でバイトを送信
 */
void stk500_send_byte(uint8_t byte);

/**
 * @brief USB CDC経由でバイトを受信
 * @param timeout_ms タイムアウト時間（ミリ秒）
 * @return 受信したバイト（タイムアウト時は-1）
 */
int stk500_receive_byte(uint32_t timeout_ms);

/**
 * @brief レスポンスヘッダーを送信
 */
void stk500_send_response_header(void);

/**
 * @brief レスポンスを送信
 */
void stk500_send_response(uint8_t status);

#ifdef __cplusplus
}
#endif

#endif /* STK500_H */