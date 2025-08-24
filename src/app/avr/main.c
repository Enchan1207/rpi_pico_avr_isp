#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#include "core/handler.h"
#include "core/logger.h"
#include "core/parser.h"

// connected to RESET# of target MCU
const unsigned int CS_PIN = 17;
const unsigned int SCK_PIN = 18;
const unsigned int MOSI_PIN = 19;
const unsigned int MISO_PIN = 16;

/// @brief パーサ・ハンドラ共用バッファ
static uint8_t buffer[259];

static uint8_t ispTransfer(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3, uint8_t cmd4) {
    uint8_t responses[4] = {0};
    // TODO

    // NOTE: 命令 ProgrammingEnable (AC 53 00 00) の場合のみ、第3バイトをレスポンスとする
    if (cmd1 == 0xAC && cmd2 == 0x53) {
        return responses[2];
    }

    return responses[3];
}

static void writeResponse(const uint8_t* response, size_t length) {
    for (size_t i = 0; i < length; i++) {
        // TODO
    }
}

static void resetTarget(bool state) {
    // TODO
}

static void sleepMillisec(uint32_t ms) {
    // TODO
}

static uint32_t setISPBaudRate(uint32_t baudRate) {
    // TODO
    return baudRate;
}

static void logOutput(const char* message) {
    // TODO
}

static bool readData(uint8_t* dst) {
    // TODO
    return false;
}

int main() {
    // TODO: GPIO configuation

    initLogger(logOutput);

    handler_context_t handlerCtx;
    initHandlerContext(&handlerCtx, ispTransfer, writeResponse, resetTarget, sleepMillisec, setISPBaudRate, buffer, sizeof(buffer));

    parser_context_t parserCtx;
    initParserContext(&parserCtx, buffer, sizeof(buffer));

    log("AVRISP avr");

    while (true) {
        resetParserState(&parserCtx);
        while (!isStateFinished(&parserCtx)) {
            processParserInput(&parserCtx, readData);
        }

        handleCommand(&parserCtx, &handlerCtx);
    }
}
