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
#include "semphr.h"

/*******************************************************************************
 * 用户LED任务相关内容定义
 *******************************************************************************
 */
#define USER_LED_TASK_PRIO 2			/* 定义任务优先级 */
#define USER_LED_TASK_STACK_SIZE 120	/* 定义任务堆栈大小 */
TaskHandle_t user_led_handle;			/* 定义任务句柄 */
void user_led_task(void *pvParameters); /* 任务函数 */

/*******************************************************************************
 * 红色LED任务相关内容定义
 *******************************************************************************
 */
#define RED_LED_TASK_PRIO 1			/* 定义任务优先级 */
#define RED_LED_TASK_STACK_SIZE 120 /* 定义任务堆栈大小 */
TaskHandle_t red_led_handle;		/* 定义任务句柄 */
void red_led_task(void *pvParameters);

/*******************************************************************************
 * 蓝色LED任务相关内容定义
 *******************************************************************************
 */
#define BLUE_LED_TASK_PRIO 1		 /* 定义任务优先级 */
#define BLUE_LED_TASK_STACK_SIZE 120 /* 定义任务堆栈大小 */
TaskHandle_t blue_led_handle;		 /* 定义任务句柄 */
void blue_led_task(void *pvParameters);

/*******************************************************************************
 * 信号量句柄定义
 *******************************************************************************
 */
SemaphoreHandle_t Semaphore1;

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

	xTaskCreate(
		(TaskFunction_t)red_led_task,
		(const char *)"red_led_task",
		(uint16_t)RED_LED_TASK_STACK_SIZE,
		(void *)NULL,
		(UBaseType_t)RED_LED_TASK_PRIO,
		(TaskHandle_t *)red_led_handle);

	xTaskCreate(
		(TaskFunction_t)blue_led_task,
		(const char *)"blue_led_task",
		(uint16_t)BLUE_LED_TASK_STACK_SIZE,
		(void *)NULL,
		(UBaseType_t)BLUE_LED_TASK_PRIO,
		(TaskHandle_t *)blue_led_handle);

	
	Semaphore1 = xSemaphoreCreateCounting(10, 0);
	
	if (Semaphore1 != NULL)
	{
		/* 信号量创建成功 */
		
	}
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
	while (1)
	{
		//blue_state = gpio_input_bit_get(BLUELED_GPIO_PORT, BLUELED_GPIO_PIN);
		user_key.handle(user_key.mode);
		//REDLED_TOGGLE;
//		if ((Semaphore1 != NULL) & (RESET==gpio_input_bit_get(USER_KEY_GPIO_PORT, USER_KEY_GPIO_PIN)))
//		{
//			xSemaphoreGive(Semaphore1);
//		}
		vTaskDelay(10);
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
	
	while (1)
	{
//		if (xSemaphoreTake(Semaphore1, ( TickType_t ) 10 ) == pdTRUE)
//		{
//			BLUELED_TOGGLE;
//		}
		//xSemaphoreTake(Semaphore1, ( TickType_t ) 10 );
		if (uxSemaphoreGetCount(Semaphore1) == 10)
		{
			for(int i=0; i<10; i++) xSemaphoreTake(Semaphore1, ( TickType_t ) 10 );
			BLUELED_TOGGLE;
		}
		
		//vTaskDelay(pdMS_TO_TICKS(100));
	}
}


/* 滴答定时器钩子函数 */
void vApplicationTickHook(void)
{
	//	/* 1sm 执行一次 */
	FlagStatus key_state = USER_KEY_STATE; /* 读取按键当前状态 */

	user_key.key_buf = (user_key.key_buf << 1) | key_state; /* key_buf 左移一位，并或上按键状态 key_state */

	/* 如果 32sm 内 32次读到的状态都为 0 代表按键按下了 */
	if ((user_key.key_buf == (uint32_t)0x00000000) && (user_key.ok_down_flag == 0))
	{
		user_key.ok_down_flag = 1;
	}

	/* 如果 ok_down_flag 等于 1  */
	if (user_key.ok_down_flag == 1)
	{
		user_key.long_down_ms++;
	}

	/* 如果 long_down_ms 小于 LONG_DOWN_FLAG_MS 则为短按 */
	if ((user_key.key_buf == (uint32_t)0xffffffff) && (user_key.long_down_ms < LONG_DOWN_FLAG_MS) && (user_key.ok_down_flag == 1))
	{
		user_key.state = CLICK; /* 单击 */
	}

	if (user_key.long_down_ms > LONG_DOWN_FLAG_MS && user_key.key_buf == (uint32_t)0x00000000)
	{
		user_key.state = HOLD_OR_LOND_DOWN;
	}
}
