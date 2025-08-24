#include "core/logger.h"

#include <stdio.h>
#include <string.h>

static LogWriteFunction writeLog = NULL;

void initLogger(LogWriteFunction logWriteFunc) {
    writeLog = logWriteFunc;
}

void debugLog(const char* file, int line, const char* func, const char* message) {
    if (writeLog == NULL) {
        return;
    }

    // ファイル名のパス部分を除去してファイル名のみ取得
    const char* filename = strrchr(file, '/');
    if (filename == NULL) {
        filename = strrchr(file, '\\');
    }
    if (filename != NULL) {
        filename++;  // '/'の次の文字を指す
    } else {
        filename = file;  // パス区切り文字が見つからない場合はそのまま使用
    }

    // ログメッセージをフォーマットして出力
    char logBuffer[256];
    snprintf(logBuffer, sizeof(logBuffer), "[%s:%d] %s(): %s\n",
             filename, line, func, message);

    writeLog(logBuffer);
}
