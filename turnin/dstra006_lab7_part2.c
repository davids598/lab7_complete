/*	Author: David Strathman
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #7  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Link To Vid: https://youtu.be/x4IwtRQ24ag 
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "../header/timer.h"
#include "../header/io.h"
//#include "io.c"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum SM_States { SM_LOOP, SM_HOLD, SM_HF } SM_State;
unsigned int iterate[] = {0x01, 0x02, 0x04};
signed char i = 0x00;
unsigned char path = 0x00; //0 = forward. 1 = backwards
unsigned char counter = 0x05;
unsigned char currA = 0x00;
unsigned char holdA = 0x00;
unsigned char holdHFA = 0x00;
void Tick() {
  switch(SM_State) {
    case SM_LOOP:
      currA = ~PINA;
      if (currA == 0x01) {
        SM_State = SM_HOLD;
      } else {
        SM_State = SM_LOOP;
      }
      break;

    case SM_HOLD:
      holdA = ~PINA;
      if (holdA == 0x00) {
        SM_State = SM_HF;
      } else {
        SM_State = SM_HOLD;
      }
      break;

    case SM_HF:
    holdHFA = ~PINA;
      if (holdHFA == 0x01) {
        SM_State = SM_LOOP;
      } else {
        SM_State = SM_HF;
      }
      break;
  }

  switch (SM_State) {
    case SM_LOOP:
      PORTB = iterate[i];

      if (path == 0x00) {
        ++i;
      }
      if (path == 0x01) {
        --i;
      }

      if (i >= 3) {
        i = 1;
        path = 0x01;
      }
      if (i <= -1) {
        i = 1;
        path = 0x00;
      }

      break;

    case SM_HOLD:
      if (PORTB == 0x02 && counter <= 8) {
        counter = counter + 1;
        LCD_Cursor(1);
        LCD_WriteData(counter + '0');
        LCD_Cursor(2);
      } else if (PORTB != 0x02 && counter >= 1) {
        counter = counter - 1;
        LCD_Cursor(1);
        LCD_WriteData(counter + '0');
        LCD_Cursor(2);
      }
      if (counter >= 9) {
        LCD_Cursor(1);
        LCD_DisplayString(1, "Nice Work");
        LCD_Cursor(12);
      }

    case SM_HF:
      if (counter >= 9) {
        counter = 5;
        LCD_ClearScreen();
        LCD_Cursor(1);
        LCD_WriteData(counter + '0');
        LCD_Cursor(2);

      }

    default:
      break;
  }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x01;
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
    DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs, initialize to 0s
    TimerSet(300);
    TimerOn();
    SM_State = SM_LOOP;
    LCD_init();
    LCD_Cursor(1);
    LCD_WriteData(counter + '0');
    LCD_Cursor(2);
    /* Insert your solution below */
    while (1) {
      Tick();
      while (!TimerFlag);
      TimerFlag = 0;
    }
    return 1;
}
