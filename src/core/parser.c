#include "core/parser.h"

#include <stddef.h>
#include <string.h>

const uint8_t STK500_EOP = 0x20;

bool isValidCommand(uint8_t commandRaw) {
    switch ((Stk500Command)commandRaw) {
        case STK500_CMD_GET_SYNC:
        case STK500_CMD_GET_SIGN_ON:
        case STK500_CMD_SET_PARAMETER:
        case STK500_CMD_GET_PARAMETER:
        case STK500_CMD_SET_DEVICE:
        case STK500_CMD_SET_DEVICE_EXT:
        case STK500_CMD_ENTER_PROG_MODE:
        case STK500_CMD_LEAVE_PROG_MODE:
        case STK500_CMD_CHIP_ERASE:
        case STK500_CMD_CHECK_AUTOINC:
        case STK500_CMD_LOAD_ADDRESS:
        case STK500_CMD_UNIVERSAL:
        case STK500_CMD_UNIVERSAL_MULTI:
        case STK500_CMD_PROG_FLASH:
        case STK500_CMD_PROG_DATA:
        case STK500_CMD_PROG_FUSE:
        case STK500_CMD_PROG_LOCK:
        case STK500_CMD_PROG_PAGE:
        case STK500_CMD_PROG_FUSE_EXT:
        case STK500_CMD_READ_FLASH:
        case STK500_CMD_READ_DATA:
        case STK500_CMD_READ_FUSE:
        case STK500_CMD_READ_LOCK:
        case STK500_CMD_READ_PAGE:
        case STK500_CMD_READ_SIGN:
        case STK500_CMD_READ_OSC_CAL:
        case STK500_CMD_READ_FUSE_EXT:
        case STK500_CMD_READ_OSC_CAL_EXT:
            return true;
        default:
            return false;
    }
}

uint8_t getCommandArgumentsLength(Stk500Command command) {
    switch (command) {
        // 引数なし
        case STK500_CMD_GET_SYNC:
        case STK500_CMD_GET_SIGN_ON:
        case STK500_CMD_ENTER_PROG_MODE:
        case STK500_CMD_LEAVE_PROG_MODE:
        case STK500_CMD_CHIP_ERASE:
        case STK500_CMD_CHECK_AUTOINC:
        case STK500_CMD_READ_FLASH:
        case STK500_CMD_READ_DATA:
        case STK500_CMD_READ_FUSE:
        case STK500_CMD_READ_LOCK:
        case STK500_CMD_READ_SIGN:
        case STK500_CMD_READ_OSC_CAL:
        case STK500_CMD_READ_FUSE_EXT:
            return 0;

        // 1バイト引数
        case STK500_CMD_GET_PARAMETER:
        case STK500_CMD_PROG_DATA:
        case STK500_CMD_PROG_LOCK:
        case STK500_CMD_READ_OSC_CAL_EXT:
            return 1;

        // 2バイト引数
        case STK500_CMD_SET_PARAMETER:
        case STK500_CMD_LOAD_ADDRESS:
        case STK500_CMD_PROG_FLASH:
        case STK500_CMD_PROG_FUSE:
            return 2;

        // 3バイト引数
        case STK500_CMD_PROG_FUSE_EXT:
        case STK500_CMD_READ_PAGE:
            return 3;

        // 4バイト引数
        case STK500_CMD_SET_DEVICE_EXT:
        case STK500_CMD_UNIVERSAL:
            return 4;

        // 20バイト引数
        case STK500_CMD_SET_DEVICE:
            return 20;

        // 可変長コマンド（最低限の引数長）
        case STK500_CMD_UNIVERSAL_MULTI:
            return 1;

        case STK500_CMD_PROG_PAGE:
            return 3;

        default:
            return 0;
    }
}

void initParserContext(parser_context_t* context) {
    if (context == NULL) {
        return;
    }

    context->state = PARSER_READY;
    context->command = STK500_CMD_GET_SYNC;
    context->expectedArgumentsLength = 0;
    context->receivedArgumentsLength = 0;
    memset(context->arguments, 0, sizeof(context->arguments));
}

ParserState processParserInput(parser_context_t* context, DataReaderFunction readFunc) {
    if (context == NULL) {
        return PARSER_READY;
    }

    if (readFunc == NULL) {
        return context->state;
    }

    // データ受信
    uint8_t data = 0;
    bool isDataReceived = readFunc(&data);

    switch (context->state) {
        case PARSER_READY:
            if (!isDataReceived) {
                break;
            }

            uint8_t argumentsLength = getCommandArgumentsLength((Stk500Command)data);

            context->command = (Stk500Command)data;
            context->expectedArgumentsLength = argumentsLength;

            if (argumentsLength == 0) {
                context->state = PARSER_EXPECTS_EOP;
            } else {
                context->state = PARSER_RECEIVE_ARGS;
            }
            break;

        case PARSER_RECEIVE_ARGS:
            if (!isDataReceived) {
                context->state = PARSER_ERROR;
                break;
            }

            // 格納してインデックスを進める
            uint16_t index = context->receivedArgumentsLength;
            context->arguments[index] = data;
            context->receivedArgumentsLength = index + 1;

            // NOTE: 可変長引数コマンドの対応

            // UniversalMulti: 第一引数がデータ長
            if (index == 0 && context->command == STK500_CMD_UNIVERSAL_MULTI) {
                // AVR061より: number_of_bytes: 0-255 equals 1-256 bytes to be sent
                uint16_t numberOfBytes = context->arguments[0] + 1;

                // 最初にデータ長を受け取っている
                context->expectedArgumentsLength = numberOfBytes;
                break;
            }

            // ProgPage: 第一,第二引数がデータ長
            if (index == 1 && context->command == STK500_CMD_PROG_PAGE) {
                uint8_t bytesHigh = context->arguments[0];
                uint8_t bytesLow = context->arguments[1];
                uint16_t numberOfBytes = (bytesHigh << 8) | bytesLow;

                // 最初にデータ長(high, low)と種別を受け取っている
                context->expectedArgumentsLength = numberOfBytes + 3;
                break;
            }

            // 全て受信したら終端待機へ
            if (context->receivedArgumentsLength == context->expectedArgumentsLength) {
                context->state = PARSER_EXPECTS_EOP;
                break;
            }

            break;

        case PARSER_EXPECTS_EOP:
            if (!isDataReceived) {
                context->state = PARSER_ERROR;
                break;
            }

            if (data != STK500_EOP) {
                context->state = PARSER_ERROR;
                break;
            }

            if (!isValidCommand(context->command)) {
                context->state = PARSER_UNKNOWN;
                break;
            }

            context->state = PARSER_ACCEPTED;
            break;

        default:
            break;
    }

    return context->state;
}
