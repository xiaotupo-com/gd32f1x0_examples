/**
 * @file        : led.h
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

/*******************************************************************************
 * 函数声明
 *******************************************************************************
 */
void led_init(void);

#endif /* __LED_H end! */
