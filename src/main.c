//
// rpi_pico_avr_isp
//

#include <hardware/spi.h>
#include <pico/stdlib.h>
#include <stdio.h>

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
    gpio_put(CS_PIN, true);  // 負論理 (negated)

    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);

    // ターゲットMCUをリセット
    gpio_put(CS_PIN, false);
    sleep_ms(20);

    // プログラミング許可命令を送信
    const uint8_t programmableCheckCmd[] = {0xAC, 0x53, 0x00, 0x00};
    uint8_t response[4] = {0};
    spi_write_read_blocking(spi, programmableCheckCmd, response, sizeof(programmableCheckCmd));

    // プログラミングを終了
    gpio_put(CS_PIN, true);

    // レスポンスをコンソールに表示
    for (uint8_t i = 0; i < 4; i++) {
        printf("%02X ", response[i]);
    }
    puts("\n");

    while (true) {
        sleep_ms(1000);
    }
}
