#ifndef __DRAW_H
#define __DRAW_H

#include "../Game/includes.h"

// ENUM DEFINITION
typedef enum { NORTH,
               NORTH_EAST,
               EAST,
               SUD_EAST,
               SUD,
               SUD_OVEST,
               OVEST,
               NORTH_OVEST,
               OVEST_NORTH_EAST } SHADOW_DIRECTION;

// FUNCTION DEFINITION
void LCD_DrawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void LCD_DrawSquare(uint16_t x0, uint16_t y0, uint16_t len, uint16_t color);
void LCD_FillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void LCD_FillSquare(uint16_t x0, uint16_t y0, uint16_t len, uint16_t color);
void LCD_DrawRectWithShadow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t rect_color, SHADOW_DIRECTION dir, uint16_t shadow_color);
void LCD_DrawShadow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SHADOW_DIRECTION dir, uint16_t color);

void LCD_ClearRect(struct Rect m_rect);
void LCD_ClearSquare(struct Vector2D m_start_pos, uint16_t m_len);

#endif
