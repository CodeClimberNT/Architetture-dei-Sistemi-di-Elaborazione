#ifndef __WALL_H__
#define __WALL_H__

#ifndef __MY_STDINT_H
#define __MY_STDINT_H
#include "stdint.h"
#endif


#include "../Game/game.h"

/*CONSTANT*/
#define WALL_DISCOUNT 1
#define WALL_WIDTH 6
#define HALF_WALL_WIDTH WALL_WIDTH / 2

/**************ENUM**************/
typedef enum { NA = 0,
               Horizontal = 1,
               Vertical = 2 } WALL_DIRECTION;

/**************STRUCT**************/

struct Wall {
  struct Vector2D position;
  WALL_DIRECTION direction;
  uint16_t color;
  uint8_t discount;
};

/**************FUNCTION**************/
void Draw_Wall(void);
struct Wall Create_Wall(struct Wall wall);
struct Wall Place_Wall(struct Wall wall);
struct Wall Preview_Wall(struct Wall wall);
struct Wall Move_Wall(struct Wall wall, DIRECTION dir);
struct Wall Rotate_Wall(struct Wall m_wall);
void Remove_Wall(struct Wall wall);
uint8_t Can_Place_Wall(struct Wall m_wall);

uint8_t Wall_Partition_Board(struct Wall m_wall);

struct Rect Get_Position_Of(struct Wall m_wall);



#endif
