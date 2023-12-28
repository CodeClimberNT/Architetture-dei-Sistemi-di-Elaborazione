
#include "button.h"
#include "lpc17xx.h"

/**
 * @brief  Function that initializes Buttons
 */
void BUTTON_init(void) {
  LPC_PINCON->PINSEL4 |= (1 << 20); /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR &= ~(1 << 10);  /* PORT2.10 defined as input          */

  LPC_PINCON->PINSEL4 |= (1 << 22); /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR &= ~(1 << 11);  /* PORT2.11 defined as input          */

  LPC_PINCON->PINSEL4 |= (1 << 24); /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR &= ~(1 << 12);  /* PORT2.12 defined as input          */

  LPC_SC->EXTMODE = 0x7;

  NVIC_EnableIRQ(EINT2_IRQn);      /* enable irq in nvic                 */
  NVIC_SetPriority(EINT2_IRQn, 1); /* priority, the lower the better     */
  NVIC_EnableIRQ(EINT1_IRQn);      /* enable irq in nvic                 */
  NVIC_SetPriority(EINT1_IRQn, 2);
  NVIC_EnableIRQ(EINT0_IRQn);      /* enable irq in nvic                 */
  NVIC_SetPriority(EINT0_IRQn, 3); /* decreasing priority	from EINT2->0	 */
}

void disable_button(uint8_t button_num) {
  __disable_irq();  // Disable interrupts globally

  switch (button_num) {
    case 0:
      NVIC_DisableIRQ(EINT0_IRQn);  // disable Interrupt
      break;
    case 1:
      NVIC_DisableIRQ(EINT1_IRQn);
      break;
    case 2:
      NVIC_DisableIRQ(EINT2_IRQn);
      break;
  }

  __enable_irq();  // Re-enable interrupts globally after configuration change
}

void enable_button(uint8_t button_num) {
  __disable_irq();  // Disable interrupts globally

  switch (button_num) {
    case 0:
      NVIC_EnableIRQ(EINT0_IRQn);
      break;
    case 1:
      NVIC_EnableIRQ(EINT1_IRQn);
      break;
    case 2:
      NVIC_EnableIRQ(EINT2_IRQn);
      break;
  }

  __enable_irq();  // Re-enable interrupts globally after configuration change
}
