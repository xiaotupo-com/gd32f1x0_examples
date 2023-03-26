/**
 * @file        : printf_debug.c
 * @brief       : printf 调试源文件
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
#include "printf_debug.h"

/**
 * @name        printf_debug_init
 * @brief       printf 调试助手初始化
 * @retval      None
 * @history     None
 * @calls       []
 * @called by   []
 * @input       []
 * @output      []
 * @others      None
 */
void printf_debug_init(void)
{
    rcu_periph_clock_enable(PRINTF_DEBUG_GPIO_CLK);  /* 初始化串口时钟 */
    rcu_periph_clock_enable(PRINTF_DEBUG_USART_CLK); /* 设置 USART 时钟使能 */

    /* 配置 Tx 和 Rx 引脚复用功能 */
    gpio_af_set(PRINTF_DEBUG_PORT, PRINTF_DEBUG_AF, PRINTF_DEBUG_RX_PIN);
    gpio_af_set(PRINTF_DEBUG_PORT, PRINTF_DEBUG_AF, PRINTF_DEBUG_TX_PIN);

    /* 配置 USART GPIO 模式*/
    gpio_mode_set(PRINTF_DEBUG_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, PRINTF_DEBUG_RX_PIN);
    gpio_output_options_set(PRINTF_DEBUG_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, PRINTF_DEBUG_RX_PIN);

    gpio_mode_set(PRINTF_DEBUG_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, PRINTF_DEBUG_TX_PIN);
    gpio_output_options_set(PRINTF_DEBUG_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, PRINTF_DEBUG_TX_PIN);

    /* USART configure */
    usart_deinit(PRINTF_DEBUG_COM);
    usart_baudrate_set(PRINTF_DEBUG_COM, PRINTF_DEBUG_COM_BAUD);
    usart_transmit_config(PRINTF_DEBUG_COM, USART_TRANSMIT_ENABLE);
    usart_receive_config(PRINTF_DEBUG_COM, USART_RECEIVE_ENABLE);
    usart_enable(PRINTF_DEBUG_COM);
}

/**
 * @name        fputc
 * @brief       将C库printf函数重新定位到USART
 * @retval      None
 * @history     None
 * @calls       []
 * @called by   []
 * @input       None
 * @output      None
 * @others      None
 */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(PRINTF_DEBUG_COM, (uint8_t)ch);
    while (RESET == usart_flag_get(PRINTF_DEBUG_COM, USART_FLAG_TBE))
        ;
    return ch;
}
