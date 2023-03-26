//==================================================
// @file 	led.h
// @brief 	LED 驱动
// @author	xiaotupo
// @site:	https://www.xiaotupo.com
// @date	2023-3-26
//==================================================
#ifndef __LED_H
#define __LED_H

/*******************************************************************************
 * 头文件包含
 *******************************************************************************
 */
#include "gd32f1x0.h"

/*******************************************************************************
 * GPIO 定义
 *******************************************************************************
 */
#define LED_GPIO_PIN GPIO_PIN_0
#define LED_GPIO_PORT GPIOB
#define LED_GPIO_CLK RCU_GPIOB

#define REDLED_GPIO_PIN GPIO_PIN_15
#define REDLED_GPIO_PORT GPIOB
#define REDLED_GPIO_CLK RCU_GPIOB

#define BLUELED_GPIO_PIN GPIO_PIN_8
#define BLUELED_GPIO_PORT GPIOA
#define BLUELED_GPIO_CLK RCU_GPIOA

/*******************************************************************************
 * 宏函数定义
 *******************************************************************************
 */
#define LED_TOGGLE GPIO_OCTL(LED_GPIO_PORT) ^= LED_GPIO_PIN /* 反正 Pin */
#define LED_ON GPIO_BC(LED_GPIO_PORT) = GPIO_BC_CR0         /* GPIO 输出低电平点亮LED */
#define LED_OFF                                    \
    {                                              \
        GPIO_BOP(LED_GPIO_PORT) &= ~GPIO_BOP_BOP0; \
        GPIO_BOP(LED_GPIO_PORT) |= GPIO_BOP_BOP0;  \
    }

#define REDLED_TOGGLE GPIO_OCTL(REDLED_GPIO_PORT) ^= REDLED_GPIO_PIN
#define BLUELED_TOGGLE GPIO_OCTL(BLUELED_GPIO_PORT) ^= BLUELED_GPIO_PIN
/*******************************************************************************
 * 函数声明
 *******************************************************************************
 */
void led_init(void);
void led_toggle(void);

#endif /* __LED_H end! */
