#include "stk500.h"

#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>

#include "avr_isp.h"
#include "commands.h"

/**
 * @brief STK500コマンド受信ステート
 */
typedef enum {
    STK_STATE_WAITING_COMMAND,
    STK_STATE_RECEIVING_PAYLOAD,
    STK_STATE_WAITING_EOP,
    STK_STATE_PROCESSING
} stk500_state_t;

/**
 * @brief STK500コマンドバッファ
 */
#define STK500_MAX_PAYLOAD_SIZE 256
typedef struct {
    uint8_t command;
    uint8_t payload[STK500_MAX_PAYLOAD_SIZE];
    uint16_t payload_length;
    uint16_t payload_received;
    uint16_t expected_payload_length;
} stk500_command_buffer_t;

/**
 * @brief プログラマーのパラメータ構造体
 */
typedef struct {
    uint16_t current_address;
    bool in_programming_mode;
    uint8_t device_parameters[20];
    uint8_t device_ext_parameters[4];
} programmer_state_t;

/**
 * @brief STK500プロトコル状態管理構造体
 */
typedef struct {
    stk500_state_t state;
    stk500_command_buffer_t command_buffer;
} stk500_protocol_state_t;

static programmer_state_t programmer_state = {0};
static stk500_protocol_state_t protocol_state = {0};

/**
 * @brief コマンドの期待ペイロード長を取得
 * @param command コマンドバイト
 * @return 期待ペイロード長（バイト数）
 */
static uint16_t get_expected_payload_length(uint8_t command) {
    switch (command) {
        case CMD_GET_SYNC:
        case CMD_GET_SIGNON:
        case CMD_ENTER_PROGMODE:
        case CMD_LEAVE_PROGMODE:
        case CMD_CHIP_ERASE:
        case CMD_CHECK_AUTOINC:
        case CMD_READ_FLASH:
        case CMD_READ_DATA:
        case CMD_READ_SIGN:
            return 0;

        case CMD_LOAD_ADDRESS:
            return 2;

        case CMD_UNIVERSAL:
            return 4;

        case CMD_SET_DEVICE:
            return 20;

        case CMD_SET_DEVICE_EXT:
            return 5;

        case CMD_PROG_PAGE:
            return 3;  // 最初の3バイト（サイズ + メモリタイプ）のみ、残りは動的に決定

        default:
            return 0;
    }
}

void stk500_init(void) {
    programmer_state.current_address = 0;
    programmer_state.in_programming_mode = false;
    memset(programmer_state.device_parameters, 0, sizeof(programmer_state.device_parameters));
    memset(programmer_state.device_ext_parameters, 0, sizeof(programmer_state.device_ext_parameters));

    protocol_state.state = STK_STATE_WAITING_COMMAND;
    memset(&protocol_state.command_buffer, 0, sizeof(protocol_state.command_buffer));
}

/**
 * @brief GetSyncコマンドを処理
 */
static void handle_get_sync(void) {
    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief GetSignOnコマンドを処理
 */
static void handle_get_signon(void) {
    const char* sign_on_message = "AVR STK";

    putchar(RESP_STK_INSYNC);
    for (int i = 0; i < strlen(sign_on_message); i++) {
        putchar(sign_on_message[i]);
    }
    putchar(RESP_STK_OK);
}

/**
 * @brief SetDeviceコマンドを処理
 */
static void handle_set_device(void) {
    memcpy(programmer_state.device_parameters, protocol_state.command_buffer.payload, 20);

    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief SetDeviceExtコマンドを処理
 */
static void handle_set_device_ext(void) {
    memcpy(programmer_state.device_ext_parameters, protocol_state.command_buffer.payload, 4);

    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief EnterProgModeコマンドを処理
 */
static void handle_enter_progmode(void) {
    if (avr_isp_enter_programming_mode()) {
        programmer_state.in_programming_mode = true;
        putchar(RESP_STK_INSYNC);
        putchar(RESP_STK_OK);
    } else {
        putchar(RESP_STK_INSYNC);
        putchar(RESP_STK_NODEVICE);
    }
}

/**
 * @brief LeaveProgModeコマンドを処理
 */
static void handle_leave_progmode(void) {
    avr_isp_leave_programming_mode();
    programmer_state.in_programming_mode = false;

    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief ChipEraseコマンドを処理
 */
static void handle_chip_erase(void) {
    if (avr_isp_chip_erase()) {
        putchar(RESP_STK_INSYNC);
        putchar(RESP_STK_OK);
    } else {
        putchar(RESP_STK_INSYNC);
        putchar(RESP_STK_FAILED);
    }
}

/**
 * @brief CheckAutoincコマンドを処理
 */
static void handle_check_autoinc(void) {
    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief LoadAddressコマンドを処理
 */
static void handle_load_address(void) {
    uint8_t addr_low = protocol_state.command_buffer.payload[0];
    uint8_t addr_high = protocol_state.command_buffer.payload[1];

    programmer_state.current_address = (uint16_t)((addr_high << 8) | addr_low);

    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief Universalコマンドを処理
 */
static void handle_universal(void) {
    uint8_t response = avr_isp_send_command(protocol_state.command_buffer.payload);

    putchar(RESP_STK_INSYNC);
    putchar(response);
    putchar(RESP_STK_OK);
}

/**
 * @brief ReadFlashコマンドを処理
 */
static void handle_read_flash(void) {
    uint8_t low_byte = avr_isp_read_flash_low(programmer_state.current_address);
    uint8_t high_byte = avr_isp_read_flash_high(programmer_state.current_address);

    programmer_state.current_address++;

    putchar(RESP_STK_INSYNC);
    putchar(low_byte);
    putchar(high_byte);
    putchar(RESP_STK_OK);
}

/**
 * @brief ReadDataコマンドを処理
 */
static void handle_read_data(void) {
    uint8_t data = avr_isp_read_eeprom(programmer_state.current_address);

    programmer_state.current_address++;

    putchar(RESP_STK_INSYNC);
    putchar(data);
    putchar(RESP_STK_OK);
}

/**
 * @brief ReadSignコマンドを処理
 */
static void handle_read_sign(void) {
    uint8_t signature[3];

    if (avr_isp_read_signature(signature)) {
        putchar(RESP_STK_INSYNC);
        putchar(signature[0]);
        putchar(signature[1]);
        putchar(signature[2]);
        putchar(RESP_STK_OK);
    } else {
        putchar(RESP_STK_INSYNC);
        putchar(RESP_STK_FAILED);
    }
}

/**
 * @brief ProgPageコマンドを処理
 */
static void handle_prog_page(void) {
    uint8_t bytes_high = protocol_state.command_buffer.payload[0];
    uint8_t bytes_low = protocol_state.command_buffer.payload[1];
    uint8_t memtype = protocol_state.command_buffer.payload[2];

    uint16_t byte_count = (uint16_t)((bytes_high << 8) | bytes_low);
    uint8_t* page_data = &protocol_state.command_buffer.payload[3];

    if (memtype == MEMTYPE_FLASH) {
        for (uint16_t i = 0; i < byte_count; i += 2) {
            uint8_t low_byte = page_data[i];
            uint8_t high_byte = page_data[i + 1];

            avr_isp_load_flash_page_low(i / 2, low_byte);
            avr_isp_load_flash_page_high(i / 2, high_byte);
        }

        avr_isp_write_flash_page(programmer_state.current_address);
    } else if (memtype == MEMTYPE_EEPROM) {
        for (uint16_t i = 0; i < byte_count; i++) {
            uint8_t data_byte = page_data[i];
            avr_isp_write_eeprom(programmer_state.current_address + i, data_byte);
        }
    }

    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief コマンド処理を実行
 */
static void execute_command(void) {
    switch (protocol_state.command_buffer.command) {
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
            putchar(RESP_STK_INSYNC);
            putchar(RESP_STK_UNKNOWN);
            break;
    }
}

bool stk500_process_commands(void) {
    int received_byte = getchar_timeout_us(2000000);

    if (received_byte == PICO_ERROR_TIMEOUT) {
        return true;
    }

    switch (protocol_state.state) {
        case STK_STATE_WAITING_COMMAND:
            protocol_state.command_buffer.command = (uint8_t)received_byte;
            protocol_state.command_buffer.expected_payload_length = get_expected_payload_length((uint8_t)received_byte);
            protocol_state.command_buffer.payload_received = 0;

            if (protocol_state.command_buffer.expected_payload_length == 0) {
                protocol_state.state = STK_STATE_WAITING_EOP;
            } else {
                protocol_state.state = STK_STATE_RECEIVING_PAYLOAD;
            }
            break;

        case STK_STATE_RECEIVING_PAYLOAD:
            protocol_state.command_buffer.payload[protocol_state.command_buffer.payload_received++] = (uint8_t)received_byte;

            if (protocol_state.command_buffer.command == CMD_PROG_PAGE &&
                protocol_state.command_buffer.payload_received == 3) {
                uint16_t page_size = (uint16_t)((protocol_state.command_buffer.payload[0] << 8) |
                                                protocol_state.command_buffer.payload[1]);
                protocol_state.command_buffer.expected_payload_length = 3 + page_size;
            }

            if (protocol_state.command_buffer.payload_received >= protocol_state.command_buffer.expected_payload_length) {
                protocol_state.state = STK_STATE_WAITING_EOP;
            }
            break;

        case STK_STATE_WAITING_EOP:
            if (received_byte == STK_EOP) {
                protocol_state.state = STK_STATE_PROCESSING;
                execute_command();
                protocol_state.state = STK_STATE_WAITING_COMMAND;
                memset(&protocol_state.command_buffer, 0, sizeof(protocol_state.command_buffer));
            } else {
                putchar(RESP_STK_NOSYNC);
                protocol_state.state = STK_STATE_WAITING_COMMAND;
                memset(&protocol_state.command_buffer, 0, sizeof(protocol_state.command_buffer));
            }
            break;

        case STK_STATE_PROCESSING:
            break;
    }

    return true;
}
