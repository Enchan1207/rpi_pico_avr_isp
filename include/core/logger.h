#ifndef LOGGER_H
#define LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

/// @brief ログ書き込み関数
typedef void (*LogWriteFunction)(const char* message);

/**
 * @brief ロガーを初期化する
 *
 * @param logWriteFunc ログ書き込み関数
 */
void initLogger(LogWriteFunction logWriteFunc);

/**
 * @brief デバッグログを出力する
 *
 * @param file ファイル名
 * @param line 行番号
 * @param func 関数名
 * @param message ログメッセージ
 */
void debugLog(const char* file, int line, const char* func, const char* message);

/// @brief ログ出力マクロ
#define log(msg) debugLog(__FILE__, __LINE__, __func__, msg)

#ifdef __cplusplus
}
#endif

#endif  // LOGGER_H
