#include "core/parser.h"
#include <stddef.h>

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