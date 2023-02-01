/**
 * @file        : keys.h
 * @brief       : KEY 按键驱动
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
#ifndef __KEYS_H
#define __KEYS_H

#ifdef __cplusplus
extern "C"
{
#endif

//******************************************************************************
//  包含头文件
//******************************************************************************
#include "gd32f1x0.h"

//******************************************************************************
//  宏定义
//******************************************************************************
#define USER_KEY_STATE gpio_input_bit_get(USER_KEY_GPIO_PORT, USER_KEY_GPIO_PIN)

#define USE_FREERTOS 0

//******************************************************************************
//  GPIO 定义
//******************************************************************************
#define USER_KEY_GPIO_PORT GPIOA
#define USER_KEY_GPIO_PIN GPIO_PIN_0
#define USER_KEY_GPIO_CLK RCU_GPIOA

//******************************************************************************
//  函数声明列表
//******************************************************************************
void keys_init(void);

#if (USE_FREERTOS == 1)
//******************************************************************************
//  使用 FreeRTOS
//******************************************************************************
#else
//******************************************************************************
//  不使用 FreeRTOS
//******************************************************************************

//******************************************************************************
//  结构体&枚举类型定义
//******************************************************************************
typedef struct
{
	bool click_state;		  /* 判断消抖后是否单击成功，TRUE: 单击成功 FALSE: 单击失败 */
	FlagStatus current_state; /* 按键消抖后的状态，RESET：低电平 SET: 高电平 */
} Key_t;

//******************************************************************************
//  extern 声明
//******************************************************************************
extern Key_t user_key;

//******************************************************************************
//  函数声明
//******************************************************************************
void keys_user_key_handle(void);

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __KEYS_H end! */
