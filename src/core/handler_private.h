#ifndef HANDLER_PRIVATE_H
#define HANDLER_PRIVATE_H

#include "core/handler.h"
#include "core/parser.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief STK500 GET_SYNC コマンドを処理する
 */
void handleGetSync(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 GET_SIGN_ON コマンドを処理する
 */
void handleGetSignOn(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 SET_PARAMETER コマンドを処理する
 */
void handleSetParameter(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 GET_PARAMETER コマンドを処理する
 */
void handleGetParameter(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 SET_DEVICE コマンドを処理する
 */
void handleSetDevice(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 SET_DEVICE_EXT コマンドを処理する
 */
void handleSetDeviceExt(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 ENTER_PROG_MODE コマンドを処理する
 */
void handleEnterProgMode(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 LEAVE_PROG_MODE コマンドを処理する
 */
void handleLeaveProgMode(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 CHIP_ERASE コマンドを処理する
 */
void handleChipErase(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 CHECK_AUTOINC コマンドを処理する
 */
void handleCheckAutoInc(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 LOAD_ADDRESS コマンドを処理する
 */
void handleLoadAddress(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 UNIVERSAL コマンドを処理する
 */
void handleUniversal(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 UNIVERSAL_MULTI コマンドを処理する
 */
void handleUniversalMulti(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 PROG_FLASH コマンドを処理する
 */
void handleProgFlash(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 PROG_DATA コマンドを処理する
 */
void handleProgData(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 PROG_FUSE コマンドを処理する
 */
void handleProgFuse(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 PROG_LOCK コマンドを処理する
 */
void handleProgLock(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 PROG_PAGE コマンドを処理する
 */
void handleProgPage(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 PROG_FUSE_EXT コマンドを処理する
 */
void handleProgFuseExt(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_FLASH コマンドを処理する
 */
void handleReadFlash(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_DATA コマンドを処理する
 */
void handleReadData(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_FUSE コマンドを処理する
 */
void handleReadFuse(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_LOCK コマンドを処理する
 */
void handleReadLock(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_PAGE コマンドを処理する
 */
void handleReadPage(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_SIGN コマンドを処理する
 */
void handleReadSign(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_OSC_CAL コマンドを処理する
 */
void handleReadOscCal(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_FUSE_EXT コマンドを処理する
 */
void handleReadFuseExt(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_OSC_CAL_EXT コマンドを処理する
 */
void handleReadOscCalExt(parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief エラー処理
 */
void handleError(parser_context_t* parserCtx, handler_context_t* handlerCtx);

#ifdef __cplusplus
}
#endif

#endif  // HANDLER_PRIVATE_H
