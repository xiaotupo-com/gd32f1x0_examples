//==================================================
// @file 	keys.c
// @brief 	按键程序源文件
// @author	xiaotupo
// @site:	https://www.xiaotupo.com
// @date	2023-3-26
//==================================================
#include <stdio.h>
#include "keys.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "system.h"


//============================================
// @brief 外部变量声明
//============================================
extern TaskHandle_t user_led_handle;
extern uint32_t led_hz;

//============================================
// @brief 定义变量
//============================================
Key_t user_key = {
	.long_down_ms = 0,
	.ok_down_flag = 0,
	.key_buf = 0xffffffff,
	.mode = HOLD_MODE,
	.state = None,
	.handle = keys_user_key_handle,
};

//==================================================
// @function 	keys_init
// @brief 		按键初始化
// @param		[]
// @calls		[rcu_periph_clock_enable(), gpio_mode_set()]
// @called by	[main()]
// @input       []
// @output		[]
// @date		2023-3-26
// @retval		[]
//==================================================
void keys_init(void)
{
	/* 使能外设时钟 */
	rcu_periph_clock_enable(USER_KEY_GPIO_CLK);

	/* 配置用户按键为输入模式，并且使能内置上拉电阻，因为板子上没有外置上拉电阻 */
	gpio_mode_set(USER_KEY_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, USER_KEY_GPIO_PIN);
}

//============================================
// @brief 使用 FreeRTOS 开始
//============================================
#if (USE_FREERTOS == 1)

//==================================================
// @function 	keys_user_key_handle
// @brief 		用户按键处理程序
// @param		mode
// ============= 按键使用那种模式
// @calls		[rcu_periph_clock_enable(), gpio_mode_set()]
// @called by	[main()]
// @input       [mode]
// @output		[]
// @date		2023-3-26
// @retval		void
//==================================================
void keys_user_key_handle(KeyMode mode)
{
	// 按键单击处理
	if (user_key.state == CLICK)
	{
		#if (ENABLE_PRINTF == 0)
		printf("CLICK\n");
		#endif
		
		//============================================
		// @brief 发送消息通知
		//============================================
		// xTaskNotifyGive(user_led_handle);
		xTaskNotify(user_led_handle, 0, eIncrement );

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
			#if (ENABLE_PRINTF == 0)
			printf("HOLD_MODE\n");
			#endif
			
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
				#if (ENABLE_PRINTF == 0)
				printf("LONG_DOWN_MODE \n");
				#endif
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

//============================================
// @brief 使用 FreeRTOS 结束
//============================================
#else




#endif // USE_FREERTOS END

//============================================
// @brief 其他内容
//============================================
