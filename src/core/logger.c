#include "core/logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static LogWriteFunction writeLog = NULL;

void initLogger(LogWriteFunction logWriteFunc) {
    writeLog = logWriteFunc;
}

void debugLog(const char* file, int line, const char* func, const char* format, ...) {
    if (writeLog == NULL) {
        return;
    }

    const char* filename = strrchr(file, '/');
    if (filename == NULL) {
        filename = strrchr(file, '\\');
    }
    if (filename != NULL) {
        filename++;
    } else {
        filename = file;
    }

    char buffer[DEBUG_BUFFER_SIZE];

    // ヘッダ
    snprintf(buffer, sizeof(buffer), "[%s:%d] %s(): ", filename, line, func);
    writeLog(buffer);

    // メッセージ
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    writeLog(buffer);

    // 改行を出力
    writeLog("\n");
}
