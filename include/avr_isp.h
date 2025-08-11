#ifndef AVR_ISP_H
#define AVR_ISP_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief AVR ISP初期化
 */
void avr_isp_init(void);

/**
 * @brief AVR ISPプログラミングモードに入る
 * @return true: 成功、false: 失敗
 */
bool avr_isp_enter_programming_mode(void);

/**
 * @brief AVR ISPプログラミングモードから抜ける
 */
void avr_isp_leave_programming_mode(void);

/**
 * @brief AVR ISPコマンドを送信
 * @param cmd 4バイトのコマンド
 * @return 4バイト目送信時のレスポンス
 */
uint8_t avr_isp_send_command(uint8_t cmd[4]);

/**
 * @brief デバイスシグネチャを読み取り
 * @param signature 3バイトのシグネチャ格納用配列
 * @return true: 成功、false: 失敗
 */
bool avr_isp_read_signature(uint8_t signature[3]);

/**
 * @brief チップイレース
 * @return true: 成功、false: 失敗
 */
bool avr_isp_chip_erase(void);

/**
 * @brief フラッシュメモリ読み込み（下位バイト）
 * @param address アドレス
 * @return 読み込んだバイト
 */
uint8_t avr_isp_read_flash_low(uint16_t address);

/**
 * @brief フラッシュメモリ読み込み（上位バイト）
 * @param address アドレス
 * @return 読み込んだバイト
 */
uint8_t avr_isp_read_flash_high(uint16_t address);

/**
 * @brief フラッシュメモリページに書き込み（下位バイト）
 * @param page_addr ページ内アドレス
 * @param data 書き込むデータ
 */
void avr_isp_load_flash_page_low(uint8_t page_addr, uint8_t data);

/**
 * @brief フラッシュメモリページに書き込み（上位バイト）
 * @param page_addr ページ内アドレス
 * @param data 書き込むデータ
 */
void avr_isp_load_flash_page_high(uint8_t page_addr, uint8_t data);

/**
 * @brief フラッシュメモリページを書き込み
 * @param address ページアドレス
 */
void avr_isp_write_flash_page(uint16_t address);

/**
 * @brief EEPROM読み込み
 * @param address アドレス
 * @return 読み込んだバイト
 */
uint8_t avr_isp_read_eeprom(uint16_t address);

/**
 * @brief EEPROM書き込み
 * @param address アドレス
 * @param data 書き込むデータ
 */
void avr_isp_write_eeprom(uint16_t address, uint8_t data);

/**
 * @brief ヒューズビット読み込み（下位）
 * @return ヒューズビット
 */
uint8_t avr_isp_read_fuse_low(void);

/**
 * @brief ヒューズビット読み込み（上位）
 * @return ヒューズビット
 */
uint8_t avr_isp_read_fuse_high(void);

/**
 * @brief 拡張ヒューズビット読み込み
 * @return 拡張ヒューズビット
 */
uint8_t avr_isp_read_fuse_ext(void);

/**
 * @brief ヒューズビット書き込み（下位）
 * @param fuse ヒューズビット
 */
void avr_isp_write_fuse_low(uint8_t fuse);

/**
 * @brief ヒューズビット書き込み（上位）
 * @param fuse ヒューズビット
 */
void avr_isp_write_fuse_high(uint8_t fuse);

/**
 * @brief 拡張ヒューズビット書き込み
 * @param fuse 拡張ヒューズビット
 */
void avr_isp_write_fuse_ext(uint8_t fuse);

/**
 * @brief ロックビット読み込み
 * @return ロックビット
 */
uint8_t avr_isp_read_lock_bits(void);

/**
 * @brief ロックビット書き込み
 * @param lock_bits ロックビット
 */
void avr_isp_write_lock_bits(uint8_t lock_bits);

/**
 * @brief 発振器キャリブレーションバイト読み込み
 * @return キャリブレーションバイト
 */
uint8_t avr_isp_read_oscillator_cal(void);

#ifdef __cplusplus
}
#endif

#endif /* AVR_ISP_H */