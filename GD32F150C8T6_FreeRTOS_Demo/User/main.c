/**
 * @file        : main.c
 * @brief       : Main 函数文件
 * @author      : 小土坡
 * @email       : xiaotupo@163.com
 * @version     : V1.0.1
 * @date        : 2023-1-7
 * @license     : 许可
 *
 * @github      : https://github.com/xiaotupo-com
 * @site        : https://xiaotupo.com
 *
 * @note        :
 *
 */
#include <stdio.h>
#include "gd32f1x0.h"
#include "systick.h"
#include "printf_debug.h"
#include "led.h"
#include "keys.h"


int main(void)
{
    /* configure systick */
    systick_config();

    /* 串口打印初始化 */
    printf_debug_init();

    /* initialize the LEDs */
    led_init();
    printf("led_init() 初始化完成！\n");

    /* 初始化按键 */
    keys_init();
    printf("keys_init() 按键初始化完成！\n");

    while (1)
    {
        keys_user_key_handle();
    }
}
