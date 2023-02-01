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

/*******************************************************************************
 * 包含头文件
 *******************************************************************************
 */
#include "gd32f1x0.h"

/*******************************************************************************
 * 宏定义
 *******************************************************************************
 */
#define USER_KEY_STATE gpio_input_bit_get(USER_KEY_GPIO_PORT, USER_KEY_GPIO_PIN)
#define LONG_DOWN_FLAG_MS (500) /* 500 ms */

#define USE_FREERTOS 0

/*******************************************************************************
 * GPIO 定义
 *******************************************************************************
 */
#define USER_KEY_GPIO_PORT GPIOA
#define USER_KEY_GPIO_PIN GPIO_PIN_0
#define USER_KEY_GPIO_CLK RCU_GPIOA

/*******************************************************************************
 * 函数声明列表
 *******************************************************************************
 */
void keys_init(void);

#if (USE_FREERTOS == 1)
/*******************************************************************************
 * 使用 FreeRTOS
 *******************************************************************************
 */
#else
/*******************************************************************************
 * 不使用 FreeRTOS
 *******************************************************************************
 */

/*******************************************************************************
 * 结构体&枚举类型定义
 *******************************************************************************
 */

typedef enum
{
	CLICK,			   /* 短按，单击 */
	HOLD_OR_LOND_DOWN, /* 长按保持 HOLD */
	None			   /* 空，为变化 */
} KeyResultState;

/* 按键模式枚举 */
typedef enum
{
	HOLD_MODE,	   /* 按住按键不松 */
	LONG_DOWN_MODE /* 长按按键超过指定时间松开后模式 */
} KeyMode;

typedef struct
{
	uint8_t ok_down_flag;	 /* 按键按下标志，在滴答定时器中使用 */
	uint32_t long_down_ms;	 /* 按键长按时间变量，在滴答定时器中使用 */
	uint32_t key_buf;		 /* 按键缓存 */
	KeyMode mode;			 /* 按键模式 */
	KeyResultState state;	 /* 按键最终的状态 */
	void (*handle)(KeyMode); /* 函数指针，按键逻辑处理 */
} Key_t;

/*******************************************************************************
 * extern 声明
 *******************************************************************************
 */
extern Key_t user_key;

/*******************************************************************************
 * 函数声明
 *******************************************************************************
 */
void keys_user_key_handle(KeyMode mode);

#endif /* USE_FREERTOS end! */

#endif /* __KEYS_H end! */
