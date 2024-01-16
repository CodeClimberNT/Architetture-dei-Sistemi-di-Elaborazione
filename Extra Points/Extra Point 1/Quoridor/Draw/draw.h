#ifndef __DRAW_H
#define __DRAW_H

#ifndef __MY_STDINT_H
#define __MY_STDINT_H
#include "stdint.h"
#endif

#include "../GLCD/GLCD.h"

/**********COLORS***********/
#define GameBG White
#define DarkGray 0x7BEF
#define ChristmasRed 0xF800
#define ChristmasGreen 0xE60
#define QuoridorRed 0xA9A9

#define P0_Color Blue2
#define P1_Color Red
#define PhantomPlayerColor DarkGray

#define WallColor 0xA9F0
#define PhantomWallColor DarkGray

/**********ENUM***********/
typedef enum { NORTH,
               NORTH_EAST,
               EAST,
               SUD_EAST,
               SUD,
               SUD_OVEST,
               OVEST,
               NORTH_OVEST,
               OVEST_NORTH_EAST } SHADOW_DIRECTION;

typedef enum { CENTER,
							 UP,
               RIGHT,
               DOWN,
               LEFT } DIRECTION;

/**********STRUCT***********/
struct Vector2D {
  int16_t x;
  int16_t y;
};

struct Rect {
  uint16_t x0;
  uint16_t y0;
  uint16_t x1;
  uint16_t y1;
};

/**********FUNCTIONS***********/
void LCD_DrawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void LCD_DrawSquare(uint16_t x0, uint16_t y0, uint16_t len, uint16_t color);
void LCD_FillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void LCD_FillSquare(uint16_t x0, uint16_t y0, uint16_t len, uint16_t color);
void LCD_DrawRectWithShadow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t rect_color, SHADOW_DIRECTION dir, uint16_t shadow_color);
void LCD_DrawShadow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SHADOW_DIRECTION dir, uint16_t color);

void LCD_ClearRect(struct Rect m_rect);
void LCD_ClearSquare(struct Vector2D m_start_pos, uint16_t m_len);

void Draw_Checkers(void);

struct Vector2D Get_Vec_From_Dir(DIRECTION dir);
DIRECTION Get_Dir_From_Vec(struct Vector2D vec2d);

#endif
