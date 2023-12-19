#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h"
#include "../timer/timer.h"
#define MAX_LENGTH 100

extern int translate_morse(unsigned char* vett_input, int vet_input_lenght, unsigned char* vett_output, int vet_output_lenght, char change_symbol, char space, char sentence_end);
//char* vett_input, Morse code vector to convert.
//int vet_input_lenght, length of the vector to convert.
//char* vett_output, ASCII output vector.
//int vet_output_lenght, length of the output vector.
//char change_symbol, the value representing the change of symbol in the input vector.
//char space, the value representing the space in the input vector.
//char sentence_end, the value representing the end of the sentence in the input vector.
int RES;

//[5,H,4,S,*,V,3,I,*,F,*,U,*,*,2,E,*,L,*,R,*,*,*,A,*,P,*,W,*,J,1]START[6,B,*,D,*,X,*,N,*,C,*,K,*,Y,8,T,7,Z,*,G,*,Q,*,M,8,*,*,O,9,*,1]
//ASCII CONVERSION
//[53,72,52,83,,86,51,73,,70,,85,,,50,69,,76,,82,,,,65,,80,,87,,74,49]START[54,66,,68,,88,,78,,67,,75,,89,56,84,55,90,,71,,81,,77,56,,,79,57,,49]
uint8_t vett_input[MAX_LENGTH]= {0,0,0,0,2,1,1,1,2,0,1,0,0,2,0,1,3,1,1,2,0,0,1,2,1,0,2,1,0,0,2,1,1,1,3,0,1,1,1,1,2,0,0,1,1,1,4};
uint8_t vett_output[MAX_LENGTH];
// 	dot=0
// 	dash=1
// 	endchar=2
// 	space=3
//	end=4

//Input Vector
uint8_t* vett_input_pt = vett_input;
int vet_input_lenght;

//Output Vector	
uint8_t* vett_output_pt = vett_output;
int vet_output_lenght ;
// Define syntax
char change_symbol = 2;
char space = 3;
char sentence_end = 4;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{
	LED_Out(0x00);
	reset_timer(0);
	reset_timer(1);
	disable_timer(0);
	disable_timer(1);
	LED_Out(0x00);
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	uint8_t curr_char = 0;
	uint8_t i = 0;
	vet_output_lenght = 0;
	//Led Off
  LED_Out(0x0);
	do{
		curr_char = vett_input[i];
		
		if(curr_char == 2 || curr_char == 3 || curr_char == 4){
			vet_output_lenght++;
			if(vet_output_lenght == 3)
				//if space count two separate char
				vet_output_lenght++;
		}

		i++;
	}while(curr_char!=4);
	//Finished Reading
	
	//Led On
	LED_Out(0xff);
	enable_timer(0);
	disable_button(0);
	disable_button(2);
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	disable_button(0);
	disable_button(1);
	RES = translate_morse(vett_input_pt, vet_input_lenght, vett_output_pt, vet_output_lenght, change_symbol, space, sentence_end);
	enable_button(0);
	enable_button(1);
	LED_Out(RES);
	enable_timer(1);
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}
