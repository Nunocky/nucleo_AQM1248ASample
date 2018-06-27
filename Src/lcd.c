#include "stm32l1xx_hal.h"

#include "lcd.h"

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;

static inline
void LCD_RS(uint8_t v) {
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, (v)? GPIO_PIN_SET:GPIO_PIN_RESET);
}

static inline
void LCD_NCS(uint8_t v) {
    HAL_GPIO_WritePin(LCD_NCS_GPIO_Port, LCD_NCS_Pin, (v)? GPIO_PIN_SET:GPIO_PIN_RESET);
}

static inline
void LCD_Command(uint8_t v)
{
    LCD_NCS(0);
    LCD_RS(0);
    HAL_SPI_Transmit(&hspi1, &v, 1, 30);
    LCD_NCS(1);
}

static inline
void LCD_Data(uint8_t *v, uint16_t length)
{
    LCD_NCS(0);
    LCD_RS(1);
    HAL_SPI_Transmit(&hspi1, v, length, 30);
    LCD_NCS(1);
}

static inline
void LCD_Data_DMA(uint8_t *v, uint16_t length)
{
    LCD_NCS(0);
    LCD_RS(1);

    HAL_SPI_Transmit_DMA(&hspi1, v, length);

    // wait for DMA transfer complete
    while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
    }
    HAL_SPI_DMAStop(&hspi1);

    LCD_NCS(1);
}

void LCD_init() 
{
    //LCD_Command(0xAE); // Display off
    LCD_displayOn(0);
    LCD_Command(0xA0); // ADC normal
    LCD_Command(0xC8); // Common output = revers
    LCD_Command(0xA3); // bias = 1/7

    LCD_Command(0x2C); // power control 1
    HAL_Delay(2);
    LCD_Command(0x2E); // power control 2
    HAL_Delay(2);
    LCD_Command(0x2F); // power control 3

    // Contrast
    LCD_Command(0x23); // Vo voltage register ratio set
    LCD_Command(0x81); // Electronic volume mode set
    LCD_Command(0x1C); // Electronic volume value set

    // Display setting
    LCD_Command(0xA4); // display all point = normal
    LCD_Command(0x40); // display start line = 0
    LCD_Command(0xA6); // display normal
    //LCD_Command(0xAF); // display on
    LCD_displayOn(1);
}

void LCD_displayOn(uint8_t v)
{
    v = (v)? 0xAF : 0x0AE;
    LCD_Command(v);
}

void LCD_setStartAddress(uint8_t addr) 
{
    uint8_t v = 0x40 | (addr & 0x3F);
    LCD_Command(v);
}

void LCD_setPageAddress(uint8_t addr) 
{
    uint8_t v = 0xB0 | (addr & 0x0F);
    LCD_Command(v);
}

void LCD_setColumnAddressH(uint8_t addr)
{
    uint8_t v = 0x10 | ((addr>>4) & 0x0F);
    LCD_Command(v);
}

void LCD_setColumnAddressL(uint8_t addr)
{
    uint8_t v = 0x00 | (addr & 0x0F);
    LCD_Command(v);
}

void LCD_setColumnAddress(uint8_t addr)
{
    LCD_setColumnAddressH(addr);
    LCD_setColumnAddressL(addr);
}

void LCD_writeDisplayData(uint8_t *data, uint16_t length)
{
//    LCD_Data(data, length);
    LCD_Data_DMA(data, length);
}

