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
#include "gd32f1x0.h"
#include "systick.h"
#include "led.h"
#include "keys.h"
#include "iic_lcd1602.h"

int main(void)
{
    /* configure systick */
    systick_config();

    /* initialize the LEDs */
    led_init();

    /* 初始化按键 */
    keys_init();
	
	/* lcd1602 初始化 */
	lcd1602_init();

    while (1)
    {
        //keys_user_key_handle();
		lcd1602_test();
		delay_1ms(200);
    }
}


