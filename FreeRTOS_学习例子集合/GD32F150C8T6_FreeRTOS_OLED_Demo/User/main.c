/**
 * @file        : main.c
 * @brief       : Main 函数文件
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
#include "gd32f1x0.h"
#include "systick.h"
#include "printf_debug.h"
#include "led.h"
#include "keys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "oled_iic.h"
#include "fonts.h"

/*******************************************************************************
 * 用户LED任务相关内容定义
 *******************************************************************************
 */
#define USER_LED_TASK_PRIO 2			/* 定义任务优先级 */
#define USER_LED_TASK_STACK_SIZE 120	/* 定义任务堆栈大小 */
TaskHandle_t user_led_handle;			/* 定义任务句柄 */
void user_led_task(void *pvParameters); /* 任务函数 */



void setup(void)
{
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
	/* 串口打印初始化 */
	printf_debug_init();

	/* initialize the LEDs */
	led_init();
	printf("led_init() 初始化完成！\n");

	/* 初始化按键 */
	keys_init();
	printf("keys_init() 按键初始化完成！\n");
	
	oled_init();
	SSD1306_Clear();

	/*******************************************************************************
	 * 创建任务
	 *******************************************************************************
	 */
	xTaskCreate(
		(TaskFunction_t)user_led_task,
		(const char *)"user_led_task",
		(uint16_t)USER_LED_TASK_STACK_SIZE,
		(void *)NULL,
		(UBaseType_t)USER_LED_TASK_PRIO,
		(TaskHandle_t *)user_led_handle);
}

int main(void)
{
	/* configure systick */
	//    systick_config();

	setup();

	/* 开启任务调度器 */
	vTaskStartScheduler();

	while (1)
	{
	}
}

/**
 * @name        user_led_task
 * @brief       用户LED任务
 * @retval      None
 * @history     None
 * @calls       []
 * @called by   []
 * @input       None
 * @output      None
 * @others      None
 */
void user_led_task(void *pvParameters)
{
	while (1)
	{
		LED_TOGGLE;
		SSD1306_GotoXY(0,0);
		SSD1306_Puts("xiaotupo", &Font_16x26, SSD1306_COLOR_BLACK);
		vTaskDelay(500);
	}
}
