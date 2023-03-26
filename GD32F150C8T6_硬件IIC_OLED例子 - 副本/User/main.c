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
#include "oled_iic.h"
#include "fonts.h"


int main(void)
{
    /* configure systick */
    systick_config();

    /* initialize the LEDs */
    led_init();

    /* 初始化按键 */
    keys_init();
	
	oled_init();
	delay_1ms(300);
	SSD1306_Clear();
	SSD1306_GotoXY(0,0);
	SSD1306_Puts("Hello", &Font_11x18, SSD1306_COLOR_BLACK);
	
	delay_1ms(200);

    while (1)
    {
        //keys_user_key_handle();
		SSD1306_Clear();
		SSD1306_GotoXY(16,0);
		SSD1306_Puts("Hello", &Font_11x18, SSD1306_COLOR_WHITE);
		delay_1ms(200);
    }
}


