/**
 * @file        : printf_debug.h
 * @brief       : printf 调试头文件
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

#ifndef __PRINTF_DEBUG_H
#define __PRINTF_DEBUG_H

#include "gd32f1x0.h"

/*******************************************************************************
 * USART 串口宏定义
 *******************************************************************************
 */
#define USART1_RX_PIN GPIO_PIN_3
#define USART1_TX_PIN GPIO_PIN_2
#define USART0_RX_PIN GPIO_PIN_10
#define USART0_TX_PIN GPIO_PIN_9
#define PRINTF_DEBUG_PORT GPIOA
#define PRINTF_DEBUG_COM USART1 /* USART0 or USART1 */
#define PRINTF_DEBUG_TX_PIN USART1_TX_PIN
#define PRINTF_DEBUG_RX_PIN USART1_RX_PIN
#define PRINTF_DEBUG_AF GPIO_AF_1
#define PRINTF_DEBUG_GPIO_CLK RCU_GPIOA   /* RCU_GPIOA */
#define PRINTF_DEBUG_USART_CLK RCU_USART1 /* USART CLK */
#define PRINTF_DEBUG_COM_BAUD 115200U /* 串口波特率设置 */

void printf_debug_init(void); /* printf 调试助手初始化 */

#endif /* __PRINTF_DEBUG_H end! */
