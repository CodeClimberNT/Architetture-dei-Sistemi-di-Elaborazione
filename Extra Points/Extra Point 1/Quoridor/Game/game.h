#ifndef __GAME_H  // Include guard
#define __GAME_H


#include "includes.h"


/*******************ENUM DEFINITION**********************/
typedef enum { TRANSITION, IDLE } GAME_STATE;

typedef enum { PLAYER,
               WALL } MOVING_ENTITY;

/*******************UTILS FUNCTIONS**********************/
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MOD(x, y) (((x % y) + y) % y)

/*******************GAME FUNCTION**********************/
void Setup(void);

void Start_Game(void);

// GAME MODE FUNCTION

void End_Turn(void);

void Switch_Player_Wall(void);

void Move(DIRECTION dir);

void wait_delay(int count);

#endif
