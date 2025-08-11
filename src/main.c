#include <hardware/spi.h>
#include <pico/stdlib.h>
#include <stdio.h>

#include "avr_isp.h"
#include "stk500.h"

/**
 * @brief メイン関数
 * AVR書込み器のメインループを実行する
 */
int main() {
    stdio_init_all();

    avr_isp_init();
    stk500_init();

    while (true) {
        stk500_process_commands();
    }

    return 0;
}
