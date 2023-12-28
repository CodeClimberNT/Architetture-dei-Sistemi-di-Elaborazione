/****************************************Copyright (c)****************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "GLCD/GLCD.h"
#include "Game/game.h"
#include "LPC17xx.h"
#include "RIT/RIT.h"
#include "button_EXINT/button.h"
#include "joystick/joystick.h"
#include "timer/timer.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag;  // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

extern GAME_STATE game_state;
extern MOVING_ENTITY moving_entity;

int main(void) {
  SystemInit(); /* System Initialization (i.e., PLL)  */
  Setup();
  LPC_SC->PCON |= 0x1; /* power-down	mode										*/
  LPC_SC->PCON &= ~(0x2);

  while (1) {
    game_state = IDLE;
    __ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
