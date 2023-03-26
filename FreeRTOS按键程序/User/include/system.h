//==================================================
// @file 	system.h
// @brief 	系统全局配置，系统参数结构体定义
// @author	xiaotupo
// @site:	https://www.xiaotupo.com
// @date	2023-3-26
//==================================================
#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "gd32f1x0.h"

//============================================
// @brief 宏定义 
//============================================
#define ENABLE_PRINTF	0					// 0: 开启 printf() 1: 关闭 printf()
#define SYSTEM_STATE_LED_NORMAL_FREQ 500 	// 系统正常时状态LED 闪烁间隔单位（ms）


//============================================
// @brief 状态枚举定义
//============================================
typedef enum
{
	NORMAL = 0, // 正常状态，状态LED在滴答定时器中 500ms 闪烁一次
	SERIAL_SEND, // 串口发送状态
	SERIAL_RECEIVE, // 串口接收状态
	
} SystemStateEnum_t;

//============================================
// @brief 系统结构体定义
//============================================
typedef struct {
	SystemStateEnum_t system_state;
} System_t;

//============================================
// @brief 声明变量
//============================================
extern System_t system;


#endif /* __SYSTEM_H END! */

