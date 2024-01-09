#ifndef __PLAYER_H
#define __PLAYER_H

#ifndef __MY_STDINT_H
#define __MY_STDINT_H
#include "stdint.h"
#endif

#include "../Game/game.h"

struct Player {
  uint8_t id;
  struct Vector2D pos;
  uint8_t wallsRemaining;
  uint16_t color;
  uint8_t ghost;
};

struct Player Create_Player(uint8_t id, uint16_t x, uint16_t y, uint16_t color, uint8_t walls);

void Draw_Player(struct Player player);
struct Player Move_Player(struct Player player, DIRECTION dir, uint8_t is_double);
void Remove_Player(struct Player player);

void Create_Hint_Move(struct Player m_player);
void Remove_Hint_Move(struct Player m_player);

void Create_Highlight(struct Player m_player);
void Remove_Highlight(struct Player m_player);

uint8_t Player_Collide_Wall(struct Vector2D p_pos, DIRECTION dir);

#endif
