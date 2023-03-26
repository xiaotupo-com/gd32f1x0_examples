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
	.click_state = FALSE,
	.current_state = SET,
};

/**
 * @name        keys_user_key_handle
 * @brief       用户按键处理程序
 * @retval      None
 * @history     None
 * @calls       []
 * @called by   []
 * @input       None
 * @output      None
 * @others      None
 */
void keys_user_key_handle(void)
{
	/* 读取单击状态 */
	if (TRUE == user_key.click_state)
	{
		LED_TOGGLE;					  /* 反转 LED 状态 */
		user_key.click_state = FALSE; /* 清除单击状态，不清除程序将不能正常工作 */
	}
}

#endif
