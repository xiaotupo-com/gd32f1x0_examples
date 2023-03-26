/**
 * @file        : led.c
 * @brief       : LED 驱动
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
#include "led.h"

/**
 * @name        led_init
 * @brief       LED 初始化
 * @retval      None
 * @history     None
 * @calls       []
 * @called by   []
 * @input       None
 * @output      None
 * @others      None
 */
void led_init(void)
{
	/* 使能 GPIO 时钟 */
	rcu_periph_clock_enable(LED_GPIO_CLK);
	rcu_periph_clock_enable(REDLED_GPIO_CLK);
	rcu_periph_clock_enable(BLUELED_GPIO_CLK);
	
	/* 配置 GPIO */
	gpio_mode_set(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GPIO_PIN);
	gpio_output_options_set(LED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_GPIO_PIN);

	gpio_mode_set(REDLED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, REDLED_GPIO_PIN);
	gpio_output_options_set(REDLED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, REDLED_GPIO_PIN);
	
	gpio_mode_set(BLUELED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, BLUELED_GPIO_PIN);
	gpio_output_options_set(BLUELED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLUELED_GPIO_PIN);
	
	// GPIO_BC(LED_GPIO_PORT) = LED_GPIO_PIN; /* 清除 LED 引脚状态为 0，BC 寄存器1: 清除输出控制寄存器 */
	GPIO_BOP(LED_GPIO_PORT) &= ~GPIO_BOP_BOP0;
	GPIO_BOP(LED_GPIO_PORT) |= GPIO_BOP_BOP0;
	
	GPIO_OCTL(REDLED_GPIO_PORT) &= ~GPIO_OCTL_OCTL15;
	GPIO_OCTL(BLUELED_GPIO_PORT) &= ~GPIO_OCTL_OCTL8;
}

/**
 * @name        led_toggle
 * @brief       LED 引脚状态反转
 * @retval      None
 * @history     None
 * @calls       []
 * @called by   []
 * @input       None
 * @output      None
 * @others      None
 */
void led_toggle(void)
{
	/* GPIO_OCTL() 宏获取GPIO 输出控制寄存器 */
	GPIO_OCTL(LED_GPIO_PORT) ^= LED_GPIO_PIN; /* 反正 Pin */
}

void led_on(void)
{
	gpio_bit_write(LED_GPIO_PORT, LED_GPIO_PIN, RESET);
}
void led_off(void)
{
	gpio_bit_write(LED_GPIO_PORT, LED_GPIO_PIN, SET);
}
