/**
 * @file		oled_iic.h
 * @brief		GD32 OLED 驱动头文件
 * @author		小土坡
 * @version		v0.01
 * @createdate	2023-3-13
 * @update		更新日期
 * @license
 * @github		https://github.com/xiaotupo-com
 * @note
 * : 该头文件为 GD32 的OLED驱动头文件
 * : 使用 gd32 的硬件IIC接口来实现通信
 */
#ifndef __OLED_IIC_H
#define __OLED_IIC_H

#include "gd32f1x0.h"
#include "stdlib.h"
#include "string.h"
#include "fonts.h"

#define OLED_IIC_ADDRESS	(0x78)

/* SSD1306 settings */
/* SSD1306 width in pixels */
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH            128
#endif
/* SSD1306 LCD height in pixels */
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT           64
#endif


/**
 * @brief  SSD1306 color enumeration
 */
typedef enum {
	SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	SSD1306_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;

void oled_init(void);
uint8_t SSD1306_Init(void);
void ssd1306_I2C_Write(uint8_t reg, uint8_t data);
void SSD1306_InvertDisplay (int i);
void SSD1306_Fill(SSD1306_COLOR_t color);
void SSD1306_UpdateScreen(void);
void SSD1306_GotoXY(uint16_t x, uint16_t y);
char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color);
char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color);
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);
void SSD1306_Clear (void);
#endif /* __OLED_IIC_H */
