#ifndef HANDLER_H
#define HANDLER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "parser.h"

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

// STK500のシステムクロックは7.3728MHz
#define STK500_SYSTEM_CLOCK 7372800

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
 * @brief RESET制御関数
 */
typedef void (*ResetControlFunction)(bool state);

/**
 * @brief スリープ関数
 */
typedef void (*SleepFunction)(uint32_t milliseconds);

/**
 * @brief ISPボーレート設定関数
 */
typedef uint32_t (*SetISPBaudRateFunction)(uint32_t baudRate);

/**
 * @brief デバイス情報
 */
typedef struct {
    /// @brief デバイスコード
    uint8_t deviceCode;

    /// @brief ロックバイト長
    uint8_t lockBytesLength;

    /// @brief ヒューズバイト長
    uint8_t fuseBytesLength;

    /// @brief フラッシュページサイズ
    uint16_t pageSize;

    /// @brief EEPROMサイズ
    uint16_t eepromSize;

    /// @brief フラッシュサイズ
    uint32_t flashSize;

    /// @brief コマンドサイズ
    uint8_t commandSize;

    /// @brief EEPROMページサイズ
    uint8_t eepromPageSize;

    /// @brief SCKクロック持続時間
    uint8_t sckDuration;
} device_info_t;

/**
 * @brief ハンドラコンテキスト
 */
typedef struct {
    /// @brief ISP書込み関数
    IspTransferFunction transfer;

    /// @brief レスポンス書き込み関数
    ResponseWriterFunction writeResponse;

    /// @brief RESET制御関数
    ResetControlFunction resetControl;

    /// @brief スリープ関数 (メモリ書き換え時の待機を想定)
    SleepFunction sleep;

    /// @brief ISPボーレート設定関数
    SetISPBaudRateFunction setISPBaudRate;

    /// @brief 現在のアドレス
    uint16_t currentAddress;

    /// @brief デバイス情報
    device_info_t deviceInfo;
} handler_context_t;

/**
 * @brief ハンドラコンテキストを初期化する
 *
 * @param context ハンドラコンテキスト
 * @param transferFunc ISP書込み関数
 * @param responseWriterFunc レスポンス書き込み関数
 * @param resetControlFunc RESET制御関数
 * @param sleepFunc スリープ関数
 * @param setISPBaudRateFunc ISPボーレート設定関数
 */
void initHandlerContext(
    handler_context_t* context,
    IspTransferFunction transferFunc,
    ResponseWriterFunction responseWriterFunc,
    ResetControlFunction resetControlFunc,
    SleepFunction sleepFunc,
    SetISPBaudRateFunction setISPBaudRateFunc);

/**
 * @brief パーサコンテキストとハンドラコンテキストを渡してコマンドを処理する
 *
 * @param parserCtx パーサコンテキスト
 * @param handlerCtx ハンドラコンテキスト
 */
void handleCommand(
    const parser_context_t* parserCtx,
    handler_context_t* handlerCtx);

/**
 * @brief SCK_DURATION値からISPボーレートを算出する
 *
 * @param sckDuration SCK_DURATION値
 * @return 算出されたボーレート
 */
static inline uint32_t calculateISPBaudRate(uint8_t sckDuration) {
    // cf: https://github.com/avrdudes/avrdude/blob/b4403627090582f6c9232920021901eebcd8bf63/src/stk500.c#L1296-L1303
    double baudRateRaw = 1.0 / (8.0 / STK500_SYSTEM_CLOCK * MAX(sckDuration, 1));
    return (uint32_t)baudRateRaw;
}

/**
 * @brief ISPボーレートからSCK_DURATION値を算出する
 *
 * @param baudRate ISPボーレート
 * @return 算出されたSCK_DURATION値
 */
static inline uint8_t calculateSCKDuration(uint32_t baudRate) {
    // cf: https://github.com/avrdudes/avrdude/blob/b4403627090582f6c9232920021901eebcd8bf63/src/stk500.c#L1320-L1328
    double durationRaw = 1.0 / baudRate;
    double duration = STK500_SYSTEM_CLOCK / 8.0 * durationRaw + 0.5;
    return (uint8_t)duration;
}

#ifdef __cplusplus
}
#endif

#endif  // HANDLER_H
