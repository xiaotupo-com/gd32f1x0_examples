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

/*******************************************************************************
 * 用户LED任务相关内容定义
 *******************************************************************************
 */
#define USER_LED_TASK_PRIO	2	/* 定义任务优先级 */
#define USER_LED_TASK_STACK_SIZE	120  /* 定义任务堆栈大小 */
TaskHandle_t user_led_handle; /* 定义任务句柄 */
void user_led_task(void *pvParameters); /* 任务函数 */

/*******************************************************************************
 * 红色LED任务相关内容定义
 *******************************************************************************
 */
#define RED_LED_TASK_PRIO	1	/* 定义任务优先级 */
#define RED_LED_TASK_STACK_SIZE	120  /* 定义任务堆栈大小 */
TaskHandle_t red_led_handle; /* 定义任务句柄 */
void red_led_task(void *pvParameters);

/*******************************************************************************
 * 蓝色LED任务相关内容定义
 *******************************************************************************
 */
#define BLUE_LED_TASK_PRIO	1	/* 定义任务优先级 */
#define BLUE_LED_TASK_STACK_SIZE	120  /* 定义任务堆栈大小 */
TaskHandle_t blue_led_handle; /* 定义任务句柄 */
void blue_led_task(void *pvParameters);

int main(void)
{
    /* configure systick */
//    systick_config();

	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
    /* 串口打印初始化 */
    printf_debug_init();

    /* initialize the LEDs */
    led_init();
    printf("led_init() 初始化完成！\n");

    /* 初始化按键 */
    keys_init();
    printf("keys_init() 按键初始化完成！\n");
	
	/*******************************************************************************
	 * 创建任务
	 *******************************************************************************
	 */
	xTaskCreate(
		(TaskFunction_t) user_led_task,
		(const char *) "user_led_task",
		(uint16_t) USER_LED_TASK_STACK_SIZE,
		(void*) NULL,
		(UBaseType_t) USER_LED_TASK_PRIO,
		(TaskHandle_t*) user_led_handle
	);
		
	xTaskCreate(
		(TaskFunction_t) red_led_task,
		(const char *) "red_led_task",
		(uint16_t) RED_LED_TASK_STACK_SIZE,
		(void*) NULL,
		(UBaseType_t) RED_LED_TASK_PRIO,
		(TaskHandle_t*) red_led_handle
	);
	
	xTaskCreate(
		(TaskFunction_t) blue_led_task,
		(const char *) "blue_led_task",
		(uint16_t) BLUE_LED_TASK_STACK_SIZE,
		(void*) NULL,
		(UBaseType_t) BLUE_LED_TASK_PRIO,
		(TaskHandle_t*) blue_led_handle
	);
		
		
	/* 开启任务调度器 */
	vTaskStartScheduler();

    while (1)
    {
//        keys_user_key_handle();
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
	while(1) {
		LED_TOGGLE;
		vTaskDelay(50);
	}
}

/**
 * @name        red_led_task
 * @brief       用户LED任务
 * @retval      None
 * @history     None
 * @calls       []
 * @called by   []
 * @input       None
 * @output      None
 * @others      None
 */
void red_led_task(void *pvParameters)
{
	while(1) {
		REDLED_TOGGLE;
		vTaskDelay(100);
	}
}

/**
 * @name        blue_led_task
 * @brief       用户LED任务
 * @retval      None
 * @history     None
 * @calls       []
 * @called by   []
 * @input       None
 * @output      None
 * @others      None
 */
void blue_led_task(void *pvParameters)
{
	static uint32_t t = 25;
	
	while(1) {
		BLUELED_TOGGLE;
		t = xTaskGetTickCount();
		printf("%d\n", t);

		
		vTaskDelay(100);
	}
}
