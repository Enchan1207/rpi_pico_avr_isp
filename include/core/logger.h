#ifndef LOGGER_H
#define LOGGER_H

#ifndef DEBUG_BUFFER_SIZE
/// @brief デバッグ用バッファサイズ
#define DEBUG_BUFFER_SIZE 64
#endif

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
 * @param format フォーマット文字列
 * @param ... 可変引数
 */
void debugLog(const char* file, int line, const char* func, const char* format, ...);

#ifndef NDEBUG
/// @brief ログ出力マクロ
#define log(format, ...) debugLog(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#else
#define log(format, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif  // LOGGER_H
