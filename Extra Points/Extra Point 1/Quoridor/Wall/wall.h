#ifndef __WALL_H
#define __WALL_H

#include "../Game/includes.h"


typedef enum { Vertical, Horizontal } WALL_DIRECTION;


struct Wall {
  struct Vector2D position;
  WALL_DIRECTION direction;
  uint16_t color;
  uint8_t discount;
};


void Draw_Wall(void);
struct Wall Create_Wall(struct Wall wall);
void Place_Wall(struct Wall wall);
struct Wall Preview_Wall(struct Wall wall);
struct Wall Move_Wall(struct Wall wall, DIRECTION dir);
struct Wall Rotate_Wall(struct Wall m_wall);
void Remove_Wall(struct Wall wall);
uint8_t Can_Place_Wall(struct Wall m_wall);
void Update_Wall(struct Rect m_rect);
struct Rect Get_Position_Of(struct Wall m_wall);

#endif
