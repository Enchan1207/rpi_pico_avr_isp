#include "avr_isp.h"

#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <pico/stdlib.h>

/**
 * @brief SPI通信用ピン定義
 */
#define CS_PIN 17    // RESET#
#define SCK_PIN 18   // SCK
#define MOSI_PIN 19  // MOSI
#define MISO_PIN 16  // MISO

/**
 * @brief AVR ISP コマンド定義
 */
#define ISP_CMD_PROGRAMMING_ENABLE {0xAC, 0x53, 0x00, 0x00}
#define ISP_CMD_CHIP_ERASE {0xAC, 0x80, 0x00, 0x00}
#define ISP_CMD_READ_SIGNATURE_0 {0x30, 0x00, 0x00, 0x00}
#define ISP_CMD_READ_SIGNATURE_1 {0x30, 0x00, 0x01, 0x00}
#define ISP_CMD_READ_SIGNATURE_2 {0x30, 0x00, 0x02, 0x00}
#define ISP_CMD_READ_FUSE_LOW {0x50, 0x00, 0x00, 0x00}
#define ISP_CMD_READ_FUSE_HIGH {0x58, 0x08, 0x00, 0x00}
#define ISP_CMD_READ_FUSE_EXT {0x50, 0x08, 0x00, 0x00}
#define ISP_CMD_READ_LOCK_BITS {0x58, 0x00, 0x00, 0x00}
#define ISP_CMD_READ_OSC_CAL {0x38, 0x00, 0x00, 0x00}

static spi_inst_t* spi = spi0;

void avr_isp_init(void) {
    spi_init(spi, 1000000);
    spi_set_format(spi, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    gpio_init(CS_PIN);
    gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_put(CS_PIN, true);

    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);
}

bool avr_isp_enter_programming_mode(void) {
    gpio_put(CS_PIN, false);
    gpio_put(SCK_PIN, false);

    sleep_ms(20);

    uint8_t cmd[] = ISP_CMD_PROGRAMMING_ENABLE;
    uint8_t response[4];

    spi_write_read_blocking(spi, cmd, response, 4);

    return response[2] == 0x53;
}

void avr_isp_leave_programming_mode(void) {
    gpio_put(CS_PIN, true);
}

uint8_t avr_isp_send_command(uint8_t cmd[4]) {
    uint8_t response[4];
    spi_write_read_blocking(spi, cmd, response, 4);
    return response[3];
}

bool avr_isp_read_signature(uint8_t signature[3]) {
    uint8_t cmd0[] = ISP_CMD_READ_SIGNATURE_0;
    uint8_t cmd1[] = ISP_CMD_READ_SIGNATURE_1;
    uint8_t cmd2[] = ISP_CMD_READ_SIGNATURE_2;

    signature[0] = avr_isp_send_command(cmd0);
    signature[1] = avr_isp_send_command(cmd1);
    signature[2] = avr_isp_send_command(cmd2);

    return true;
}

bool avr_isp_chip_erase(void) {
    uint8_t cmd[] = ISP_CMD_CHIP_ERASE;
    avr_isp_send_command(cmd);

    sleep_ms(50);
    return true;
}

uint8_t avr_isp_read_flash_low(uint16_t address) {
    uint8_t cmd[] = {0x20, (uint8_t)(address >> 8), (uint8_t)(address & 0xFF), 0x00};
    return avr_isp_send_command(cmd);
}

uint8_t avr_isp_read_flash_high(uint16_t address) {
    uint8_t cmd[] = {0x28, (uint8_t)(address >> 8), (uint8_t)(address & 0xFF), 0x00};
    return avr_isp_send_command(cmd);
}

void avr_isp_load_flash_page_low(uint8_t pageAddr, uint8_t data) {
    uint8_t cmd[] = {0x40, 0x00, pageAddr, data};
    avr_isp_send_command(cmd);
}

void avr_isp_load_flash_page_high(uint8_t pageAddr, uint8_t data) {
    uint8_t cmd[] = {0x48, 0x00, pageAddr, data};
    avr_isp_send_command(cmd);
}

void avr_isp_write_flash_page(uint16_t address) {
    uint8_t cmd[] = {0x4C, (uint8_t)(address >> 8), (uint8_t)(address & 0xFF), 0x00};
    avr_isp_send_command(cmd);

    sleep_ms(50);
}

uint8_t avr_isp_read_eeprom(uint16_t address) {
    uint8_t cmd[] = {0xA0, (uint8_t)(address >> 8), (uint8_t)(address & 0xFF), 0x00};
    return avr_isp_send_command(cmd);
}

void avr_isp_write_eeprom(uint16_t address, uint8_t data) {
    uint8_t cmd[] = {0xC0, (uint8_t)(address >> 8), (uint8_t)(address & 0xFF), data};
    avr_isp_send_command(cmd);

    sleep_ms(10);
}

uint8_t avr_isp_read_fuse_low(void) {
    uint8_t cmd[] = ISP_CMD_READ_FUSE_LOW;
    return avr_isp_send_command(cmd);
}

uint8_t avr_isp_read_fuse_high(void) {
    uint8_t cmd[] = ISP_CMD_READ_FUSE_HIGH;
    return avr_isp_send_command(cmd);
}

uint8_t avr_isp_read_fuse_ext(void) {
    uint8_t cmd[] = ISP_CMD_READ_FUSE_EXT;
    return avr_isp_send_command(cmd);
}

void avr_isp_write_fuse_low(uint8_t fuse) {
    uint8_t cmd[] = {0xAC, 0xA0, 0x00, fuse};
    avr_isp_send_command(cmd);
    sleep_ms(50);
}

void avr_isp_write_fuse_high(uint8_t fuse) {
    uint8_t cmd[] = {0xAC, 0xA8, 0x00, fuse};
    avr_isp_send_command(cmd);
    sleep_ms(50);
}

void avr_isp_write_fuse_ext(uint8_t fuse) {
    uint8_t cmd[] = {0xAC, 0xA4, 0x00, fuse};
    avr_isp_send_command(cmd);
    sleep_ms(50);
}

uint8_t avr_isp_read_lockBits(void) {
    uint8_t cmd[] = ISP_CMD_READ_LOCK_BITS;
    return avr_isp_send_command(cmd);
}

void avr_isp_write_lockBits(uint8_t lockBits) {
    uint8_t cmd[] = {0xAC, 0xE0, 0x00, lockBits};
    avr_isp_send_command(cmd);
    sleep_ms(50);
}

uint8_t avr_isp_read_oscillator_cal(void) {
    uint8_t cmd[] = ISP_CMD_READ_OSC_CAL;
    return avr_isp_send_command(cmd);
}