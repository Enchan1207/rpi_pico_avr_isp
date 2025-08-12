#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 二つの値を加算する
 *
 * @param lhs
 * @param rhs
 * @return uint8_t
 */
uint8_t add(uint8_t lhs, uint8_t rhs);

#ifdef __cplusplus
}
#endif

#endif  // PARSER_H
