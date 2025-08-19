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
void handleGetSync(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 GET_SIGN_ON コマンドを処理する
 */
void handleGetSignOn(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 SET_PARAMETER コマンドを処理する
 */
void handleSetParameter(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 GET_PARAMETER コマンドを処理する
 */
void handleGetParameter(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 SET_DEVICE コマンドを処理する
 */
void handleSetDevice(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 SET_DEVICE_EXT コマンドを処理する
 */
void handleSetDeviceExt(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 ENTER_PROG_MODE コマンドを処理する
 */
void handleEnterProgMode(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 LEAVE_PROG_MODE コマンドを処理する
 */
void handleLeaveProgMode(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 CHIP_ERASE コマンドを処理する
 */
void handleChipErase(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 CHECK_AUTOINC コマンドを処理する
 */
void handleCheckAutoInc(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 LOAD_ADDRESS コマンドを処理する
 */
void handleLoadAddress(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 UNIVERSAL コマンドを処理する
 */
void handleUniversal(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 UNIVERSAL_MULTI コマンドを処理する
 */
void handleUniversalMulti(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 PROG_FLASH コマンドを処理する
 */
void handleProgFlash(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 PROG_DATA コマンドを処理する
 */
void handleProgData(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 PROG_FUSE コマンドを処理する
 */
void handleProgFuse(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 PROG_LOCK コマンドを処理する
 */
void handleProgLock(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 PROG_PAGE コマンドを処理する
 */
void handleProgPage(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 PROG_FUSE_EXT コマンドを処理する
 */
void handleProgFuseExt(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_FLASH コマンドを処理する
 */
void handleReadFlash(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_DATA コマンドを処理する
 */
void handleReadData(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_FUSE コマンドを処理する
 */
void handleReadFuse(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_LOCK コマンドを処理する
 */
void handleReadLock(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_PAGE コマンドを処理する
 */
void handleReadPage(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_SIGN コマンドを処理する
 */
void handleReadSign(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_OSC_CAL コマンドを処理する
 */
void handleReadOscCal(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_FUSE_EXT コマンドを処理する
 */
void handleReadFuseExt(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief STK500 READ_OSC_CAL_EXT コマンドを処理する
 */
void handleReadOscCalExt(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief エラー処理
 */
void handleError(const parser_context_t* parserCtx, handler_context_t* handlerCtx);

/**
 * @brief 現在のアドレスが属するページの開始アドレスを取得する
 */
uint16_t getCurrentPage(const handler_context_t* handlerCtx);

#ifdef __cplusplus
}
#endif

#endif  // HANDLER_PRIVATE_H
