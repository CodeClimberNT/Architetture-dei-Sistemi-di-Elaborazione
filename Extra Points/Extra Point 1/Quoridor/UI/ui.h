#ifndef __UI_H
#define __UI_H

#include "../Game/includes.h"

// STRUCT DEFINITION
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


void Waiting_UI(void);

void UI_Init(void);

void UI_Counter_Init(void);

void UI_Init(void);
struct UI Create_UI(uint8_t id, uint16_t ui_x, uint16_t ui_y, uint16_t height, uint16_t width, uint16_t tit_x, uint16_t tit_y, char title_text[2], uint16_t val_x, uint16_t val_y, char value_text[2]);

void Update_UI(struct UI ui);

void Update_Timer(uint8_t time_value);

void Decrease_Timer(void);

#endif
