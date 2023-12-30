#ifndef __FIGURES_H
#define __FIGURES_H

#include "../Game/includes.h"



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


struct Vector2D Get_Relative_Pos(DIRECTION dir);
#endif
