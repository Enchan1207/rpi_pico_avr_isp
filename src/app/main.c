#include <hardware/spi.h>
#include <pico/stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "core/handler.h"
#include "core/parser.h"

// connected to RESET# of target MCU
const unsigned int CS_PIN = 17;
const unsigned int SCK_PIN = 18;
const unsigned int MOSI_PIN = 19;
const unsigned int MISO_PIN = 16;

static spi_inst_t* spiInstance;

static uint8_t ispTransfer(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3, uint8_t cmd4) {
    uint8_t responses[4] = {0};
    spi_write_read_blocking(spi0, (uint8_t[]){cmd1, cmd2, cmd3, cmd4}, responses, sizeof(responses));

    // NOTE: 命令 ProgrammingEnable (AC 53 00 00) の場合のみ、第3バイトをレスポンスとする
    if (cmd1 == 0xAC && cmd2 == 0x53) {
        return responses[2];
    }

    return responses[3];
}

static inline void writeResponse(const uint8_t* response, size_t length) {
    for (size_t i = 0; i < length; i++) {
        stdio_putchar_raw(response[i]);
    }
}

static inline void resetTarget(bool state) {
    gpio_put(CS_PIN, state);
}

static uint32_t setISPBaudRate(uint32_t baudRate) {
    return spi_set_baudrate(spiInstance, baudRate);
}

static bool readData(uint8_t* dst) {
    int result = stdio_getchar_timeout_us(2000000);
    if (result == PICO_ERROR_TIMEOUT) {
        return false;
    }

    *dst = (uint8_t)result;
    return true;
}

int main() {
    stdio_init_all();

    spiInstance = spi0;
    spi_init(spiInstance, calculateISPBaudRate(0));
    spi_set_format(spiInstance, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    gpio_init(CS_PIN);
    gpio_set_dir(CS_PIN, GPIO_OUT);

    // 負論理
    gpio_put(CS_PIN, true);

    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);

    handler_context_t handlerCtx;
    initHandlerContext(&handlerCtx, ispTransfer, writeResponse, resetTarget, sleep_ms, setISPBaudRate);

    parser_context_t parserCtx;

    while (true) {
        initParserContext(&parserCtx);
        while (!isStateFinished(&parserCtx)) {
            processParserInput(&parserCtx, readData);
        }

        handleCommand(&parserCtx, &handlerCtx);
    }
}
