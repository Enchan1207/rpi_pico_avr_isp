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
    uint16_t payloadLength;
    uint16_t payloadReceived;
    uint16_t expectedPayloadLength;
} stk500_command_buffer_t;

/**
 * @brief プログラマーのパラメータ構造体
 */
typedef struct {
    uint16_t currentAddress;
    bool inProgrammingMode;
    uint8_t deviceParameters[20];
    uint8_t deviceExtParameters[4];
} programmerState_t;

/**
 * @brief STK500プロトコル状態管理構造体
 */
typedef struct {
    stk500_state_t state;
    stk500_command_buffer_t commandBuffer;
} stk500_protocolState_t;

static programmerState_t programmerState = {0};
static stk500_protocolState_t protocolState = {0};

/**
 * @brief コマンドの期待ペイロード長を取得
 * @param command コマンドバイト
 * @return 期待ペイロード長（バイト数）
 */
static uint16_t getExpectedPayloadLength(uint8_t command) {
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

void stk500Init(void) {
    programmerState.currentAddress = 0;
    programmerState.inProgrammingMode = false;
    memset(programmerState.deviceParameters, 0, sizeof(programmerState.deviceParameters));
    memset(programmerState.deviceExtParameters, 0, sizeof(programmerState.deviceExtParameters));

    protocolState.state = STK_STATE_WAITING_COMMAND;
    memset(&protocolState.commandBuffer, 0, sizeof(protocolState.commandBuffer));
}

/**
 * @brief GetSyncコマンドを処理
 */
static void handleGetSync(void) {
    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief GetSignOnコマンドを処理
 */
static void handleGetSignon(void) {
    const char* signOnMessage = "AVR STK";

    putchar(RESP_STK_INSYNC);
    for (int i = 0; i < strlen(signOnMessage); i++) {
        putchar(signOnMessage[i]);
    }
    putchar(RESP_STK_OK);
}

/**
 * @brief SetDeviceコマンドを処理
 */
static void handleSetDevice(void) {
    memcpy(programmerState.deviceParameters, protocolState.commandBuffer.payload, 20);

    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief SetDeviceExtコマンドを処理
 */
static void handleSetDeviceExt(void) {
    memcpy(programmerState.deviceExtParameters, protocolState.commandBuffer.payload, 4);

    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief EnterProgModeコマンドを処理
 */
static void handleEnterProgmode(void) {
    if (avr_isp_enter_programming_mode()) {
        programmerState.inProgrammingMode = true;
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
static void handleLeaveProgmode(void) {
    avr_isp_leave_programming_mode();
    programmerState.inProgrammingMode = false;

    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief ChipEraseコマンドを処理
 */
static void handleChipErase(void) {
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
static void handleCheckAutoinc(void) {
    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief LoadAddressコマンドを処理
 */
static void handleLoadAddress(void) {
    uint8_t addrLow = protocolState.commandBuffer.payload[0];
    uint8_t addrHigh = protocolState.commandBuffer.payload[1];

    programmerState.currentAddress = (uint16_t)((addrHigh << 8) | addrLow);

    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief Universalコマンドを処理
 */
static void handleUniversal(void) {
    uint8_t response = avr_isp_send_command(protocolState.commandBuffer.payload);

    putchar(RESP_STK_INSYNC);
    putchar(response);
    putchar(RESP_STK_OK);
}

/**
 * @brief ReadFlashコマンドを処理
 */
static void handleReadFlash(void) {
    uint8_t lowByte = avr_isp_read_flash_low(programmerState.currentAddress);
    uint8_t highByte = avr_isp_read_flash_high(programmerState.currentAddress);

    programmerState.currentAddress++;

    putchar(RESP_STK_INSYNC);
    putchar(lowByte);
    putchar(highByte);
    putchar(RESP_STK_OK);
}

/**
 * @brief ReadDataコマンドを処理
 */
static void handleReadData(void) {
    uint8_t data = avr_isp_read_eeprom(programmerState.currentAddress);

    programmerState.currentAddress++;

    putchar(RESP_STK_INSYNC);
    putchar(data);
    putchar(RESP_STK_OK);
}

/**
 * @brief ReadSignコマンドを処理
 */
static void handleReadSign(void) {
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
static void handleProgPage(void) {
    uint8_t bytesHigh = protocolState.commandBuffer.payload[0];
    uint8_t bytesLow = protocolState.commandBuffer.payload[1];
    uint8_t memtype = protocolState.commandBuffer.payload[2];

    uint16_t byteCount = (uint16_t)((bytesHigh << 8) | bytesLow);
    uint8_t* pageData = &protocolState.commandBuffer.payload[3];

    if (memtype == MEMTYPE_FLASH) {
        for (uint16_t i = 0; i < byteCount; i += 2) {
            uint8_t lowByte = pageData[i];
            uint8_t highByte = pageData[i + 1];

            avr_isp_load_flash_page_low(i / 2, lowByte);
            avr_isp_load_flash_page_high(i / 2, highByte);
        }

        avr_isp_write_flash_page(programmerState.currentAddress);
    } else if (memtype == MEMTYPE_EEPROM) {
        for (uint16_t i = 0; i < byteCount; i++) {
            uint8_t dataByte = pageData[i];
            avr_isp_write_eeprom(programmerState.currentAddress + i, dataByte);
        }
    }

    putchar(RESP_STK_INSYNC);
    putchar(RESP_STK_OK);
}

/**
 * @brief コマンド処理を実行
 */
static void executeCommand(void) {
    switch (protocolState.commandBuffer.command) {
        case CMD_GET_SYNC:
            handleGetSync();
            break;

        case CMD_GET_SIGNON:
            handleGetSignon();
            break;

        case CMD_SET_DEVICE:
            handleSetDevice();
            break;

        case CMD_SET_DEVICE_EXT:
            handleSetDeviceExt();
            break;

        case CMD_ENTER_PROGMODE:
            handleEnterProgmode();
            break;

        case CMD_LEAVE_PROGMODE:
            handleLeaveProgmode();
            break;

        case CMD_CHIP_ERASE:
            handleChipErase();
            break;

        case CMD_CHECK_AUTOINC:
            handleCheckAutoinc();
            break;

        case CMD_LOAD_ADDRESS:
            handleLoadAddress();
            break;

        case CMD_UNIVERSAL:
            handleUniversal();
            break;

        case CMD_READ_FLASH:
            handleReadFlash();
            break;

        case CMD_READ_DATA:
            handleReadData();
            break;

        case CMD_READ_SIGN:
            handleReadSign();
            break;

        case CMD_PROG_PAGE:
            handleProgPage();
            break;

        default:
            putchar(RESP_STK_INSYNC);
            putchar(RESP_STK_UNKNOWN);
            break;
    }
}

bool stk500ProcessCommands(void) {
    int receivedByte = getchar_timeout_us(2000000);

    if (receivedByte == PICO_ERROR_TIMEOUT) {
        return true;
    }

    switch (protocolState.state) {
        case STK_STATE_WAITING_COMMAND:
            protocolState.commandBuffer.command = (uint8_t)receivedByte;
            protocolState.commandBuffer.expectedPayloadLength = getExpectedPayloadLength((uint8_t)receivedByte);
            protocolState.commandBuffer.payloadReceived = 0;

            if (protocolState.commandBuffer.expectedPayloadLength == 0) {
                protocolState.state = STK_STATE_WAITING_EOP;
            } else {
                protocolState.state = STK_STATE_RECEIVING_PAYLOAD;
            }
            break;

        case STK_STATE_RECEIVING_PAYLOAD:
            protocolState.commandBuffer.payload[protocolState.commandBuffer.payloadReceived++] = (uint8_t)receivedByte;

            if (protocolState.commandBuffer.command == CMD_PROG_PAGE &&
                protocolState.commandBuffer.payloadReceived == 3) {
                uint16_t pageSize = (uint16_t)((protocolState.commandBuffer.payload[0] << 8) |
                                               protocolState.commandBuffer.payload[1]);
                protocolState.commandBuffer.expectedPayloadLength = 3 + pageSize;
            }

            if (protocolState.commandBuffer.payloadReceived >= protocolState.commandBuffer.expectedPayloadLength) {
                protocolState.state = STK_STATE_WAITING_EOP;
            }
            break;

        case STK_STATE_WAITING_EOP:
            if (receivedByte == STK_EOP) {
                protocolState.state = STK_STATE_PROCESSING;
                executeCommand();
                protocolState.state = STK_STATE_WAITING_COMMAND;
                memset(&protocolState.commandBuffer, 0, sizeof(protocolState.commandBuffer));
            } else {
                putchar(RESP_STK_NOSYNC);
                protocolState.state = STK_STATE_WAITING_COMMAND;
                memset(&protocolState.commandBuffer, 0, sizeof(protocolState.commandBuffer));
            }
            break;

        case STK_STATE_PROCESSING:
            break;
    }

    return true;
}
