#include "oled_iic.h"
#include "gd32f1x0.h"



// PB8->I2C0_SCL
// PB9->I2C0_SDA

#define IIC_SDA_PORT	GPIOB
#define IIC_SCL_PORT	GPIOB
#define IIC_SDA_PIN		GPIO_PIN_9
#define IIC_SCL_PIN		GPIO_PIN_8
#define IIC_GPIO_RCU		RCU_GPIOB
#define IICX		I2C0
#define IICX_RCU	RCU_I2C0

/* Write command */
#define SSD1306_WRITECOMMAND(command)      ssd1306_I2C_Write(0x00, (command))
/* Write data */
#define SSD1306_WRITEDATA(data)            ssd1306_I2C_Write(0x40, (data))
/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))

/* SSD1306 data buffer */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

/* Private SSD1306 structure */
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;


/* Private variable */
static SSD1306_t SSD1306;

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define SSD1306_DEACTIVATE_SCROLL                    0x2E // Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F // Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range

#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7


static void gpio_init(void);
static void iic_init(void);
static void iic_write_byte(uint8_t byte);



void oled_init(void)
{
	gpio_init();
	iic_init();
	SSD1306_Init();
}

static void gpio_init(void)
{
	/* enable GPIOB clock */
    rcu_periph_clock_enable(IIC_GPIO_RCU);
    /* enable I2C clock */
    rcu_periph_clock_enable(IICX_RCU);
	
	/* I2C GPIO ports */
    /* connect I2C_SCL_GPIO_PIN to I2C_SCL */
    gpio_af_set(IIC_SCL_PORT, GPIO_AF_1, IIC_SCL_PIN);
    /* connect I2C_SDA_GPIO_PIN to I2C_SDA */
    gpio_af_set(IIC_SDA_PORT, GPIO_AF_1, IIC_SDA_PIN);
    /* configure GPIO pins of I2C */
    gpio_mode_set(IIC_SCL_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, IIC_SCL_PIN);
    gpio_output_options_set(IIC_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, IIC_SCL_PIN);
    gpio_mode_set(IIC_SDA_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, IIC_SDA_PIN);
    gpio_output_options_set(IIC_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, IIC_SDA_PIN);

}

static void iic_init(void)
{
	/* configure I2C clock */
    i2c_clock_config(IICX, 100000, I2C_DTCY_2);
    /* configure I2C address */
    i2c_mode_addr_config(IICX, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, OLED_IIC_ADDRESS);
    /* enable I2CX */
    i2c_enable(IICX);
    /* enable acknowledge */
    i2c_ack_config(IICX, I2C_ACK_ENABLE);
}

static void iic_write_byte(uint8_t byte)
{
	/* 等待IIC总线空闲 */
    while(i2c_flag_get(IICX, I2C_FLAG_I2CBSY));
	
	/* 发送Start开始信号 */
    i2c_start_on_bus(IICX);
	
	/* wait until SBSEND bit is set */
	/* 在主模式下发送启动条件 */
    while(!i2c_flag_get(IICX, I2C_FLAG_SBSEND));
	
	/* send slave address to I2C bus */
	/* 发送从地址到I2C总线 */
    i2c_master_addressing(IICX, OLED_IIC_ADDRESS, I2C_TRANSMITTER);
	
	/* wait until ADDSEND bit is set */
	/* 等待直到设置ADDSEND位 */
    while(!i2c_flag_get(IICX, I2C_FLAG_ADDSEND));
	
	/* clear ADDSEND bit */
    i2c_flag_clear(IICX, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while(!i2c_flag_get(IICX, I2C_FLAG_TBE));
	
	/* data transmission */
    i2c_data_transmit(IICX, byte);
    /* wait until the TBE bit is set */
    while(!i2c_flag_get(IICX, I2C_FLAG_TBE));
	
	/* send a stop condition to I2C bus */
    i2c_stop_on_bus(IICX);
    while(I2C_CTL0(IICX) & I2C_CTL0_STOP);
}

void SSD1306_Fill(SSD1306_COLOR_t color) {
	/* Set memory */
	memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}


uint8_t SSD1306_Init(void) {
	/* Init LCD */
	SSD1306_WRITECOMMAND(0xAE); //display off
	SSD1306_WRITECOMMAND(0x20); //Set Memory Addressing Mode   
	SSD1306_WRITECOMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	SSD1306_WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	SSD1306_WRITECOMMAND(0xC8); //Set COM Output Scan Direction
	SSD1306_WRITECOMMAND(0x00); //---set low column address
	SSD1306_WRITECOMMAND(0x10); //---set high column address
	SSD1306_WRITECOMMAND(0x40); //--set start line address
	SSD1306_WRITECOMMAND(0x81); //--set contrast control register
	SSD1306_WRITECOMMAND(0xFF);
	SSD1306_WRITECOMMAND(0xA1); //--set segment re-map 0 to 127
	SSD1306_WRITECOMMAND(0xA6); //--set normal display
	SSD1306_WRITECOMMAND(0xA8); //--set multiplex ratio(1 to 64)
	SSD1306_WRITECOMMAND(0x3F); //
	SSD1306_WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	SSD1306_WRITECOMMAND(0xD3); //-set display offset
	SSD1306_WRITECOMMAND(0x00); //-not offset
	SSD1306_WRITECOMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
	SSD1306_WRITECOMMAND(0xF0); //--set divide ratio
	SSD1306_WRITECOMMAND(0xD9); //--set pre-charge period
	SSD1306_WRITECOMMAND(0x22); //
	SSD1306_WRITECOMMAND(0xDA); //--set com pins hardware configuration
	SSD1306_WRITECOMMAND(0x12);
	SSD1306_WRITECOMMAND(0xDB); //--set vcomh
	SSD1306_WRITECOMMAND(0x20); //0x20,0.77xVcc
	SSD1306_WRITECOMMAND(0x8D); //--set DC-DC enable
	SSD1306_WRITECOMMAND(0x14); //
	SSD1306_WRITECOMMAND(0xAF); //--turn on SSD1306 panel
	

	SSD1306_WRITECOMMAND(SSD1306_DEACTIVATE_SCROLL);

	/* Clear screen */
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	
	/* Update screen */
	SSD1306_UpdateScreen();
	
	/* Set default values */
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;
	
	/* Initialized OK */
	SSD1306.Initialized = 1;
	
	/* Return OK */
	return 1;
}

void ssd1306_I2C_WriteMulti(uint8_t reg, uint8_t* data, uint16_t count)
{
	iic_write_byte(reg);
	
	for (uint8_t i=0; i<count; i++)
	{
		iic_write_byte(data[i]);
	}
}

void ssd1306_I2C_Write(uint8_t reg, uint8_t data) {
	
	iic_write_byte(reg);
	iic_write_byte(data);
}

void SSD1306_InvertDisplay (int i)
{
  if (i) SSD1306_WRITECOMMAND (SSD1306_INVERTDISPLAY);

  else SSD1306_WRITECOMMAND (SSD1306_NORMALDISPLAY);

}

void SSD1306_UpdateScreen(void) {
	uint8_t m;
	
	for (m = 0; m < 8; m++) {
		SSD1306_WRITECOMMAND(0xB0 + m);
		SSD1306_WRITECOMMAND(0x00);
		SSD1306_WRITECOMMAND(0x10);
		
		/* Write multi data */
		ssd1306_I2C_WriteMulti(0x40, &SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
	}
}

void SSD1306_GotoXY(uint16_t x, uint16_t y) {
	/* Set write pointers */
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color) {
	uint32_t i, b, j;
	
	/* Check available space in LCD */
	if (
		SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)
	) {
		/* Error */
		return 0;
	}
	
	/* Go through font */
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t) color);
			} else {
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
			}
		}
	}
	
	/* Increase pointer */
	SSD1306.CurrentX += Font->FontWidth;
	
	/* Return character written */
	return ch;
}

char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (SSD1306_Putc(*str, Font, color) != *str) {
			/* Return error */
			return *str;
		}
		
		/* Increase string pointer */
		str++;
	}
	
	/* Everything OK, zero should be returned */
	return *str;
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color) {
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Error */
		return;
	}
	
	/* Check if pixels are inverted */
	if (SSD1306.Inverted) {
		color = (SSD1306_COLOR_t)!color;
	}
	
	/* Set color */
	if (color == SSD1306_COLOR_WHITE) {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	} else {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

void SSD1306_Clear (void)
{
	SSD1306_Fill (0);
    SSD1306_UpdateScreen();
}
