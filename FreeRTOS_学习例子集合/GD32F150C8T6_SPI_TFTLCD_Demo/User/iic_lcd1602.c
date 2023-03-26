#include "iic_lcd1602.h"
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

static void gpio_init(void);
static void iic_init(void);
static void iic_write_byte(uint8_t byte);


void lcd1602_init(void)
{
	gpio_init();
	iic_init();
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
    i2c_mode_addr_config(IICX, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, LCD1602_ADDRESS);
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
    while(!i2c_flag_get(IICX, I2C_FLAG_SBSEND));
	
	/* send slave address to I2C bus */
    i2c_master_addressing(IICX, LCD1602_ADDRESS, I2C_TRANSMITTER);
	
	/* wait until ADDSEND bit is set */
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

void lcd1602_test(void)
{
	iic_write_byte(0x80);
	iic_write_byte(0x81);
	iic_write_byte(0x82);
	iic_write_byte(0x83);
	iic_write_byte(0x84);
	
	

}
