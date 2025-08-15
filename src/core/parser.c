#include "core/parser.h"

#include <stddef.h>
#include <string.h>

/// @brief コマンド終端
static const uint8_t STK500_EOP = 0x20;

bool getCommandArgumentsLength(Stk500Command command, uint8_t* length) {
    if (length == NULL) {
        return false;
    }

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
            *length = 0;
            return true;

        // 1バイト引数
        case STK500_CMD_GET_PARAMETER:
        case STK500_CMD_PROG_DATA:
        case STK500_CMD_PROG_LOCK:
        case STK500_CMD_READ_OSC_CAL_EXT:
            *length = 1;
            return true;

        // 2バイト引数
        case STK500_CMD_SET_PARAMETER:
        case STK500_CMD_LOAD_ADDRESS:
        case STK500_CMD_PROG_FLASH:
        case STK500_CMD_PROG_FUSE:
            *length = 2;
            return true;

        // 3バイト引数
        case STK500_CMD_PROG_FUSE_EXT:
        case STK500_CMD_READ_PAGE:
            *length = 3;
            return true;

        // 4バイト引数
        case STK500_CMD_SET_DEVICE_EXT:
        case STK500_CMD_UNIVERSAL:
            *length = 4;
            return true;

        // 20バイト引数
        case STK500_CMD_SET_DEVICE:
            *length = 20;
            return true;

        // 可変長コマンド（最低限の引数長）
        case STK500_CMD_UNIVERSAL_MULTI:
            *length = 1;
            return true;

        case STK500_CMD_PROG_PAGE:
            *length = 3;
            return true;

        default:
            return false;
    }
}

void initParserContext(parser_context_t* context) {
    if (context == NULL) {
        return;
    }

    context->state = PARSER_READY;
    context->expectedArgumentsLength = 0;
    context->receivedArgumentsLength = 0;
}

void processParserInput(parser_context_t* context, UartReadFunction readFunc) {
    if (context == NULL || readFunc == NULL) {
        return;
    }

    // データ受信
    uint8_t data = 0;
    bool readResult = readFunc(&data);

    switch (context->state) {
        case PARSER_READY:
            break;

        case PARSER_RECEIVE_ARGS:
            break;

        case PARSER_EXPECTS_EOP:
            break;

        case PARSER_ACCEPTED:
            break;

        case PARSER_UNKNOWN:
            break;

        case PARSER_ERROR:
            break;
    }
}
