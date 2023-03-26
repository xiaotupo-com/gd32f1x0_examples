//==================================================
// @file 	main.c
// @brief 	程序 main() 函数文件
// @author	xiaotupo
// @site:	https://www.xiaotupo.com
// @date	2023-3-26
//==================================================
#include <stdio.h>
#include "gd32f1x0.h"
#include "systick.h"
#include "printf_debug.h"
#include "led.h"
#include "keys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "system.h"

uint32_t led_hz = 10;

/*******************************************************************************
 * 用户LED任务相关内容定义
 *******************************************************************************
 */
#define USER_LED_TASK_PRIO 1			/* 定义任务优先级 */
#define USER_LED_TASK_STACK_SIZE 128	/* 定义任务堆栈大小 */
TaskHandle_t user_led_handle;			/* 定义任务句柄 */
void user_led_task(void *pvParameters); /* 任务函数 */




/*******************************************************************************
 * 按键任务相关内容定义
 *******************************************************************************
 */
#define KEY_TASK_PRIO 2
#define KEY_TASK_STACK_SIZE 128
TaskHandle_t key_task_handle;
void key_task(void *pvParameters);

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
        (TaskFunction_t)user_led_task,
        (const char *)"user_led_task",
        (uint16_t)USER_LED_TASK_STACK_SIZE,
        (void *)NULL,
        (UBaseType_t)USER_LED_TASK_PRIO,
        &user_led_handle);

    xTaskCreate(
        (TaskFunction_t)key_task,
        (const char *)"key_task",
        (uint16_t)KEY_TASK_STACK_SIZE,
        (void *)NULL,
        (UBaseType_t)KEY_TASK_PRIO,
        &key_task_handle);

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
    static uint32_t ulNotifyValue;
    BaseType_t xReturn = pdTRUE;
    while (1)
    {
        /* 等待用户按键单击通知 */
        //ulNotifyValue = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        xReturn = xTaskNotifyWait(0x00, 0x00, &ulNotifyValue,portMAX_DELAY);

        if (xReturn == pdTRUE) {
            if (ulNotifyValue < 10)
            {
                led_toggle();
            }
            printf("ulNotifyValue: %d\n", ulNotifyValue);
            if (ulNotifyValue>30)
            {
                ulNotifyValue = 0;
                //xTaskNotifyStateClearIndexed(user_led_handle, );
                //xTaskNotifyStateClear(NULL);
            }
        }
    }
}

/**
 * @name        key_task
 * @brief       按键任务
 * @retval      None
 * @history     None
 * @calls       []
 * @called by   []
 * @input       None
 * @output      None
 * @others      None
 */
void key_task(void *pvParameters)
{
    while (1)
    {
        user_key.handle(user_key.mode);
        vTaskDelay(pdMS_TO_TICKS(100));
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

    //============================================
    // 系统状态 LED 闪烁
    //============================================
    static int system_state_freq = SYSTEM_STATE_LED_NORMAL_FREQ;
    system_state_freq--;
    if ((system_state_freq == 0) && (system.system_state == NORMAL))
    {
        led_toggle();
        system_state_freq = SYSTEM_STATE_LED_NORMAL_FREQ;
    }
}
