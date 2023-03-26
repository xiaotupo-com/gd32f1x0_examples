//==================================================
// @file 	keys.h
// @brief 	按键程序头文件
// @author	xiaotupo
// @site:	https://www.xiaotupo.com
// @date	2023-3-26
//==================================================
#ifndef __KEYS_H
#define __KEYS_H

//============================================
// @brief 包含头文件
//============================================
#include "gd32f1x0.h"

//============================================
// @brief 宏定义
//============================================
#define USER_KEY_STATE gpio_input_bit_get(USER_KEY_GPIO_PORT, USER_KEY_GPIO_PIN)
#define LONG_DOWN_FLAG_MS (500) // 按键长按时间

#define USE_FREERTOS 1

//============================================
// @brief IO口定义
//============================================
#define USER_KEY_GPIO_PORT GPIOA
#define USER_KEY_GPIO_PIN GPIO_PIN_0
#define USER_KEY_GPIO_CLK RCU_GPIOA

//============================================
// @brief 按键枚举与结构体
//============================================
typedef enum
{
	CLICK,			   /* 短按，单击 */
	HOLD_OR_LOND_DOWN, /* 长按保持 HOLD */
	None			   /* 空，无变化 */
} KeyResultState;

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

//============================================
// @brief 函数声明列表
//============================================
void keys_init(void);

//============================================
// @brief 使用 FreeRTOS 开始
//============================================
#if (USE_FREERTOS == 1)


//============================================
// @brief 使用 FreeRTOS 结束
//============================================
#else


#endif /* USE_FREERTOS end! */

//============================================
// @brief 函数声明
//============================================
void keys_user_key_handle(KeyMode mode);

//============================================
// @brief 变量声明
//============================================
extern Key_t user_key;

#endif /* __KEYS_H end! */
