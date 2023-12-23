#ifndef __PLAYER_H  // Include guard
#define __PLAYER_H

#ifndef __STDINT_H
#include <stdint.h>
#endif

#define MAX_WALLS	8
#define MAX_TIME	20 //seconds


struct Point {
    uint16_t x;
    uint16_t y;
};

struct Player {
    uint8_t Id;
    struct Point Position;
    uint8_t wallsRemaining; 
};

void Player_Init(void);
struct Player Create_Player(uint8_t Id);
uint8_t Update_Wall_Counter(uint8_t Id, uint8_t value);

#endif  // __PLAYER_H
