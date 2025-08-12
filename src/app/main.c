#include <hardware/spi.h>
#include <pico/stdlib.h>
#include <stdio.h>

#include "core/calc.h"

// connected to RESET# of target MCU
const unsigned int CS_PIN = 17;
const unsigned int SCK_PIN = 18;
const unsigned int MOSI_PIN = 19;
const unsigned int MISO_PIN = 16;

int main() {
    stdio_init_all();

    spi_inst_t* spi = spi0;
    spi_init(spi, 1000000);
    spi_set_format(spi, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    gpio_init(CS_PIN);
    gpio_set_dir(CS_PIN, GPIO_OUT);

    // 負論理
    gpio_put(CS_PIN, true);

    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);

    uint8_t x = 5;
    uint8_t y = 8;
    uint8_t f = add(x, y);
    printf("%d\n", f);

    while (true) {
        sleep_ms(1000);
    }
}
