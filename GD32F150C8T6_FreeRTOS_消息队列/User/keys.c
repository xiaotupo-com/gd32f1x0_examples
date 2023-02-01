/**
 * @file        : keys.c
 * @brief       : 按键驱动
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
#include "keys.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t red_led_handle;
extern uint32_t led_hz;

/**
 * @name        keys_init
 * @brief       按键初始化
 * @retval      None
 * @history     None
 * @calls       []
 * @called by   []
 * @input       None
 * @output      None
 * @others      None
 */
void keys_init(void)
{
	/* 使能外设时钟 */
	rcu_periph_clock_enable(USER_KEY_GPIO_CLK);

	/* 配置用户按键为输入模式，并且使能内置上拉电阻，因为板子上没有外置上拉电阻 */
	gpio_mode_set(USER_KEY_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, USER_KEY_GPIO_PIN);
}

/* 判断是否使用 FreeRTOS */
#if (USE_FREERTOS == 1)

#else
/*******************************************************************************
 * 不使用 FreeRTOS
 *******************************************************************************
 */

/*******************************************************************************
 * 定义全局变量
 *******************************************************************************
 */

/* 定义一个按键对象 */
Key_t user_key = {
	.long_down_ms = 0,
	.ok_down_flag = 0,
	.key_buf = 0xffffffff,
	.mode = HOLD_MODE,
	.state = None,
	.handle = keys_user_key_handle,
};

/**
 * @name        keys_user_key_handle
 * @brief       用户按键处理程序
 * @retval      None
 * @history     None
 * @calls       []
 * @called by   [main.c->key_task()]
 * @input       None
 * @output      None
 * @others      None
 */
void keys_user_key_handle(KeyMode mode)
{
	static uint8_t dir = 0;
	/* 按键单击处理 */
	if (user_key.state == CLICK)
	{
		printf("CLICK\n");

		xTaskNotifyGive(red_led_handle);

		user_key.long_down_ms = 0;
		user_key.ok_down_flag = 0;
		user_key.state = None;
	}

	if (user_key.state == HOLD_OR_LOND_DOWN)
	{
		/* 判断按键模式 */
		switch (mode)
		{
		case HOLD_MODE: /* 按住按键不松开 */
			printf("HOLD_MODE\n");
			/* 闪烁频率减小 */
			if (led_hz <= 250 && dir == 0)
			{
				if (led_hz == 250)
					dir = 1;
				led_hz++;
			}
			/* 闪烁频率增大 */
			if (led_hz >= 10 && dir == 1)
			{
				if (led_hz == 10)
					dir = 0;
				led_hz--;
			}

			vTaskDelay(10);

			if (user_key.key_buf == 0xffffffff)
			{
				user_key.long_down_ms = 0;
				user_key.ok_down_flag = 0;
				user_key.state = None;
			}
			break;
		case LONG_DOWN_MODE: /* 按住按键超过指定时长后松开 */
			if (user_key.key_buf == 0xffffffff)
			{
				printf("LONG_DOWN_MODE \n");
				user_key.long_down_ms = 0;
				user_key.ok_down_flag = 0;
				user_key.state = None;
			}
			break;

		default:
			break;
		}
	}
}

#endif
