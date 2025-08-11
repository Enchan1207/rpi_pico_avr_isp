#include "stk500.h"

#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>

#include "avr_isp.h"
#include "commands.h"

/**
 * @brief プログラマーのパラメータ構造体
 */
typedef struct {
    uint16_t current_address;
    bool in_programming_mode;
    uint8_t device_parameters[20];
    uint8_t device_ext_parameters[4];
} programmer_state_t;

static programmer_state_t programmer_state = {0};

void stk500_init(void) {
    programmer_state.current_address = 0;
    programmer_state.in_programming_mode = false;
    memset(programmer_state.device_parameters, 0, sizeof(programmer_state.device_parameters));
    memset(programmer_state.device_ext_parameters, 0, sizeof(programmer_state.device_ext_parameters));
}

void stk500_send_byte(uint8_t byte) {
    putchar(byte);
}

int stk500_receive_byte(uint32_t timeout_ms) {
    int c = getchar_timeout_us(2000000);

    if (c != PICO_ERROR_TIMEOUT) {
        return c;
    }

    return -1;
}

void stk500_send_response_header(void) {
    stk500_send_byte(RESP_STK_INSYNC);
}

void stk500_send_response(uint8_t status) {
    stk500_send_byte(status);
}

/**
 * @brief GetSyncコマンドを処理
 */
static void handle_get_sync(void) {
    stk500_send_response_header();
    stk500_send_response(RESP_STK_OK);
}

/**
 * @brief GetSignOnコマンドを処理
 */
static void handle_get_signon(void) {
    const char* sign_on_message = "AVR STK";

    stk500_send_response_header();
    for (int i = 0; i < strlen(sign_on_message); i++) {
        stk500_send_byte(sign_on_message[i]);
    }
    stk500_send_response(RESP_STK_OK);
}

/**
 * @brief SetDeviceコマンドを処理
 */
static void handle_set_device(void) {
    for (int i = 0; i < 20; i++) {
        int byte = stk500_receive_byte(2000);
        if (byte < 0) {
            stk500_send_response(RESP_STK_FAILED);
            return;
        }
        programmer_state.device_parameters[i] = (uint8_t)byte;
    }

    stk500_send_response_header();
    stk500_send_response(RESP_STK_OK);
}

/**
 * @brief SetDeviceExtコマンドを処理
 */
static void handle_set_device_ext(void) {
    for (int i = 0; i < 5; i++) {
        int byte = stk500_receive_byte(2000);
        if (byte < 0) {
            stk500_send_response(RESP_STK_FAILED);
            return;
        }
        if (i < 4) {
            programmer_state.device_ext_parameters[i] = (uint8_t)byte;
        }
    }

    stk500_send_response_header();
    stk500_send_response(RESP_STK_OK);
}

/**
 * @brief EnterProgModeコマンドを処理
 */
static void handle_enter_progmode(void) {
    if (avr_isp_enter_programming_mode()) {
        programmer_state.in_programming_mode = true;
        stk500_send_response_header();
        stk500_send_response(RESP_STK_OK);
    } else {
        stk500_send_response_header();
        stk500_send_response(RESP_STK_NODEVICE);
    }
}

/**
 * @brief LeaveProgModeコマンドを処理
 */
static void handle_leave_progmode(void) {
    avr_isp_leave_programming_mode();
    programmer_state.in_programming_mode = false;

    stk500_send_response_header();
    stk500_send_response(RESP_STK_OK);
}

/**
 * @brief ChipEraseコマンドを処理
 */
static void handle_chip_erase(void) {
    if (avr_isp_chip_erase()) {
        stk500_send_response_header();
        stk500_send_response(RESP_STK_OK);
    } else {
        stk500_send_response_header();
        stk500_send_response(RESP_STK_FAILED);
    }
}

/**
 * @brief CheckAutoincコマンドを処理
 */
static void handle_check_autoinc(void) {
    stk500_send_response_header();
    stk500_send_response(RESP_STK_OK);
}

/**
 * @brief LoadAddressコマンドを処理
 */
static void handle_load_address(void) {
    int addr_low = stk500_receive_byte(2000);
    int addr_high = stk500_receive_byte(2000);

    if (addr_low < 0 || addr_high < 0) {
        stk500_send_response(RESP_STK_FAILED);
        return;
    }

    programmer_state.current_address = (uint16_t)((addr_high << 8) | addr_low);

    stk500_send_response_header();
    stk500_send_response(RESP_STK_OK);
}

/**
 * @brief Universalコマンドを処理
 */
static void handle_universal(void) {
    uint8_t cmd[4];

    for (int i = 0; i < 4; i++) {
        int byte = stk500_receive_byte(2000);
        if (byte < 0) {
            stk500_send_response(RESP_STK_FAILED);
            return;
        }
        cmd[i] = (uint8_t)byte;
    }

    uint8_t response = avr_isp_send_command(cmd);

    stk500_send_response_header();
    stk500_send_byte(response);
    stk500_send_response(RESP_STK_OK);
}

/**
 * @brief ReadFlashコマンドを処理
 */
static void handle_read_flash(void) {
    uint8_t low_byte = avr_isp_read_flash_low(programmer_state.current_address);
    uint8_t high_byte = avr_isp_read_flash_high(programmer_state.current_address);

    programmer_state.current_address++;

    stk500_send_response_header();
    stk500_send_byte(low_byte);
    stk500_send_byte(high_byte);
    stk500_send_response(RESP_STK_OK);
}

/**
 * @brief ReadDataコマンドを処理
 */
static void handle_read_data(void) {
    uint8_t data = avr_isp_read_eeprom(programmer_state.current_address);

    programmer_state.current_address++;

    stk500_send_response_header();
    stk500_send_byte(data);
    stk500_send_response(RESP_STK_OK);
}

/**
 * @brief ReadSignコマンドを処理
 */
static void handle_read_sign(void) {
    uint8_t signature[3];

    if (avr_isp_read_signature(signature)) {
        stk500_send_response_header();
        stk500_send_byte(signature[0]);
        stk500_send_byte(signature[1]);
        stk500_send_byte(signature[2]);
        stk500_send_response(RESP_STK_OK);
    } else {
        stk500_send_response_header();
        stk500_send_response(RESP_STK_FAILED);
    }
}

/**
 * @brief ProgPageコマンドを処理
 */
static void handle_prog_page(void) {
    int bytes_high = stk500_receive_byte(2000);
    int bytes_low = stk500_receive_byte(2000);
    int memtype = stk500_receive_byte(2000);

    if (bytes_high < 0 || bytes_low < 0 || memtype < 0) {
        stk500_send_response(RESP_STK_FAILED);
        return;
    }

    uint16_t byte_count = (uint16_t)((bytes_high << 8) | bytes_low);

    if (memtype == MEMTYPE_FLASH) {
        for (uint16_t i = 0; i < byte_count; i += 2) {
            int low_byte = stk500_receive_byte(2000);
            int high_byte = stk500_receive_byte(2000);

            if (low_byte < 0 || high_byte < 0) {
                stk500_send_response(RESP_STK_FAILED);
                return;
            }

            avr_isp_load_flash_page_low(i / 2, (uint8_t)low_byte);
            avr_isp_load_flash_page_high(i / 2, (uint8_t)high_byte);
        }

        avr_isp_write_flash_page(programmer_state.current_address);
    } else if (memtype == MEMTYPE_EEPROM) {
        for (uint16_t i = 0; i < byte_count; i++) {
            int data_byte = stk500_receive_byte(2000);
            if (data_byte < 0) {
                stk500_send_response(RESP_STK_FAILED);
                return;
            }

            avr_isp_write_eeprom(programmer_state.current_address + i, (uint8_t)data_byte);
        }
    }

    stk500_send_response_header();
    stk500_send_response(RESP_STK_OK);
}

bool stk500_process_commands(void) {
    int command = stk500_receive_byte(2000);

    if (command < 0) {
        return true;
    }

    switch (command) {
        case CMD_GET_SYNC:
            handle_get_sync();
            break;

        case CMD_GET_SIGNON:
            handle_get_signon();
            break;

        case CMD_SET_DEVICE:
            handle_set_device();
            break;

        case CMD_SET_DEVICE_EXT:
            handle_set_device_ext();
            break;

        case CMD_ENTER_PROGMODE:
            handle_enter_progmode();
            break;

        case CMD_LEAVE_PROGMODE:
            handle_leave_progmode();
            break;

        case CMD_CHIP_ERASE:
            handle_chip_erase();
            break;

        case CMD_CHECK_AUTOINC:
            handle_check_autoinc();
            break;

        case CMD_LOAD_ADDRESS:
            handle_load_address();
            break;

        case CMD_UNIVERSAL:
            handle_universal();
            break;

        case CMD_READ_FLASH:
            handle_read_flash();
            break;

        case CMD_READ_DATA:
            handle_read_data();
            break;

        case CMD_READ_SIGN:
            handle_read_sign();
            break;

        case CMD_PROG_PAGE:
            handle_prog_page();
            break;

        default:
            stk500_send_response_header();
            stk500_send_response(RESP_STK_UNKNOWN);
            break;
    }

    int eop = stk500_receive_byte(2000);
    if (eop != STK_EOP) {
        stk500_send_byte(RESP_STK_NOSYNC);
    }

    return true;
}