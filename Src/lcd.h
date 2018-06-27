#ifndef LCD_H
#define LCD_H

#include "stdint.h"

void LCD_init();
void LCD_displayOn(uint8_t v);
void LCD_setStartAddress(uint8_t addr);
void LCD_setPageAddress(uint8_t addr);
void LCD_setColumnAddressH(uint8_t addr);
void LCD_setColumnAddressL(uint8_t addr);
void LCD_setColumnAddress(uint8_t addr);
void LCD_writeDisplayData(uint8_t *data, uint16_t length);


#endif
