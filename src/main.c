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
    uint8_t programmableCheckResponse[4] = {0};
    spi_write_read_blocking(spi, programmableCheckCmd, programmableCheckResponse, sizeof(programmableCheckCmd));

    // 正しく送信できていれば、第3バイトに0x53が入るはず
    if (programmableCheckResponse[2] != 0x53) {
        puts("not sync");
        return 1;
    }
    puts("MCU connected");

    // デバイス識別子を取得
    uint8_t identifierGetCmd[] = {0x30, 0x00, 0x00};
    uint8_t identifierGetResponse[3] = {0};
    for (uint8_t i = 0; i < sizeof(identifierGetResponse); i++) {
        identifierGetCmd[2] = i;
        spi_write_blocking(spi, identifierGetCmd, sizeof(identifierGetCmd));
        spi_read_blocking(spi, 0x00, identifierGetResponse + i, 1);
    }

    // ヒューズビットを取得 (ext -> high -> low)
    uint8_t fuseGetResponse[3] = {0};

    const uint8_t extFuseGetCmd[] = {0x50, 0x08, 0x00};
    spi_write_blocking(spi, extFuseGetCmd, sizeof(extFuseGetCmd));
    spi_read_blocking(spi, 0x00, fuseGetResponse, 1);

    const uint8_t highFuseGetCmd[] = {0x58, 0x08, 0x00};
    spi_write_blocking(spi, highFuseGetCmd, sizeof(highFuseGetCmd));
    spi_read_blocking(spi, 0x00, fuseGetResponse + 1, 1);

    const uint8_t lowFuseGetCmd[] = {0x50, 0x00, 0x00};
    spi_write_blocking(spi, lowFuseGetCmd, sizeof(lowFuseGetCmd));
    spi_read_blocking(spi, 0x00, fuseGetResponse + 2, 1);

    // プログラミングを終了
    gpio_put(CS_PIN, true);

    // デバイス識別子をコンソールに表示
    printf("device identifier: ");
    for (uint8_t i = 0; i < sizeof(identifierGetResponse); i++) {
        printf("%02X ", identifierGetResponse[i]);
    }
    puts("");

    // ヒューズビットをコンソールに表示
    printf("fuse bits(ext, high, low): ");
    for (uint8_t i = 0; i < sizeof(fuseGetResponse); i++) {
        printf("%02X ", fuseGetResponse[i]);
    }
    puts("");

    while (true) {
        sleep_ms(1000);
    }
}
