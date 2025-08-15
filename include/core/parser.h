#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdint.h>

#include "stk500.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief コマンドパーサの状態
typedef enum {
    /// @brief 準備完了, 受信待機中
    PARSER_READY = 0,

    /// @brief 引数受信中
    PARSER_RECEIVE_ARGS,

    /// @brief 終端待機
    PARSER_EXPECTS_EOP,

    /// @brief 受理
    PARSER_ACCEPTED,

    /// @brief 不明なコマンド
    PARSER_UNKNOWN,

    /// @brief その他のパースエラー
    PARSER_ERROR
} ParserState;

/// @brief パーサのコンテキスト
typedef struct {
    /// @brief 現在のパーサの状態
    ParserState state;

    /// @brief コマンド
    Stk500Command command;

    /// @brief 期待される引数の長さ
    uint16_t expectedArgumentsLength;

    /// @brief 受信した引数の長さ
    uint16_t receivedArgumentsLength;

    /// @brief 引数バッファ
    uint8_t arguments[259];
} parser_context_t;

/// @brief 与えられたコードが有効なコマンドかどうかを返す
bool isValidCommand(uint8_t commandRaw);

/**
 * @brief 与えられたコマンドの引数長を返す
 *
 * @param command STK500コマンド
 * @return uint8_t 引数長 (不正なコマンドの場合は0)
 *
 * @note 引数内にデータ全体長の情報が含まれる可変長のコマンドについては、
 *       最低限必要な引数の長さを返します。
 */
uint8_t getCommandArgumentsLength(Stk500Command command);

/**
 * @brief パーサのコンテキストを初期化する
 *
 * @param context
 */
void initParserContext(parser_context_t* context);

/**
 * @brief UARTからデータを読み取る関数
 */
typedef bool (*UartReadFunction)(uint8_t* data);

/**
 * @brief パーサに入力を与えて処理する
 *
 * @param context コンテキスト
 * @param readFunc 入力を返す関数
 * @return ParserState 処理後のパーサ状態
 */
ParserState processParserInput(
    parser_context_t* context,
    UartReadFunction readFunc);

#ifdef __cplusplus
}
#endif

#endif  // PARSER_H
