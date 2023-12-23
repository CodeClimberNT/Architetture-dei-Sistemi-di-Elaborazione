#ifndef __DRAW_FUNCT_H  // Include guard
#define __DRAW_FUNCT_H

#ifndef __STDINT_H
#include <stdint.h>
#endif

void LCD_DrawRect( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void LCD_DrawSquare( uint16_t x0, uint16_t y0, uint16_t len, uint16_t color );
void LCD_FillRect( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void LCD_FillSquare( uint16_t x0, uint16_t y0, uint16_t len, uint16_t color );
void Draw_Wall(uint16_t x, uint16_t y, WALL_DIRECTION wall_dir);
#endif  // __DRAW_FUNCT_H
