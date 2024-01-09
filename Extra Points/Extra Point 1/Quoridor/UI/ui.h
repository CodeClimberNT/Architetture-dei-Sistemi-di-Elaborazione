#ifndef __UI_H
#define __UI_H

#ifndef __MY_STDINT_H
#define __MY_STDINT_H
#include "stdint.h"
#endif

#include "../Game/game.h"

/**********CONSTANCT***************/
#define BIG_UI_WIDTH  MAX_X * 0.8
#define BIG_UI_HEIGHT 80

#define BIG_UI_XPOS (MAX_X - BIG_UI_WIDTH) / 2   // CENTER HORIZONTALLY
#define BIG_UI_YPOS (MAX_Y - BIG_UI_HEIGHT) / 2  // CENTER VERTICALLY

/**********STRUCT***************/
struct UI {
  uint8_t id;
  struct Vector2D ui_Position;
  struct Vector2D title_position;
  char *title_text;
  struct Vector2D value_position;
  char *value_text;
  uint16_t height;
  uint16_t width;
};


struct UI Create_UI(uint8_t id, uint16_t ui_x, uint16_t ui_y, uint16_t height, uint16_t width, uint16_t tit_x, uint16_t tit_y, char title_text[2], uint16_t val_x, uint16_t val_y, char value_text[2]);
void Update_UI(struct UI ui);
void Update_Timer_UI(uint8_t time_value);
void Update_Wall_UI(struct UI m_player_ui, uint8_t wall_value);

void Show_Big_UI(int16_t txt_xoff, int16_t txt_yoff, uint8_t *p_text, SHADOW_DIRECTION shadow_dir, uint8_t temp_show, uint32_t delay, uint16_t border_color);

void Write_Center_Top(uint8_t *p_text);
void Write_Center_Bottom(uint8_t *p_text);
void Write_Top_Title(uint8_t *p_text, uint16_t color);
void Write_Bottom_Title(uint8_t *p_first_text, uint16_t first_color, uint8_t *p_second_text, uint16_t second_color);

#endif
