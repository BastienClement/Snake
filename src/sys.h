#ifndef SYS_H
#define SYS_H 1

#include <stdlib.h>
#include <c8051f020.h>	// inclure fichier définition SFR

#define SYSCLK 22118400

//-----------------------------------------------------------------------------
// BITx macros
//-----------------------------------------------------------------------------

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

//-----------------------------------------------------------------------------
// Fixed length types
//-----------------------------------------------------------------------------

typedef unsigned char 	uint8;		// 8 bits non signé
typedef signed char 	int8;		// 8 bits signé
typedef unsigned int	uint16;		// 16 bits non signé
typedef signed int 		int16;		// 16 bits signé
typedef unsigned long 	uint32;		// 32 bits non signé
typedef signed long 	int32;		// 32 bits signé

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F00x
//-----------------------------------------------------------------------------

sfr16 DP       = 0x82;                 // data pointer
sfr16 TMR3RL   = 0x92;                 // Timer3 reload value
sfr16 TMR3     = 0x94;                 // Timer3 counter
sfr16 ADC0     = 0xbe;                 // ADC0 data
sfr16 ADC0GT   = 0xc4;                 // ADC0 greater than window
sfr16 ADC0LT   = 0xc6;                 // ADC0 less than window
sfr16 RCAP2    = 0xca;                 // Timer2 capture/reload
sfr16 T2       = 0xcc;                 // Timer2
sfr16 DAC0     = 0xd2;                 // DAC0 data
sfr16 DAC1     = 0xd5;                 // DAC1 data

//-----------------------------------------------------------------------------
// Types
//-----------------------------------------------------------------------------

typedef bit bool;
#define true	1
#define false	0

typedef unsigned char byte;

//-----------------------------------------------------------------------------
// Helpers
//-----------------------------------------------------------------------------

extern byte i;
extern byte j;
extern byte k;

#define foreach(var, from, to) for(var = from; var < to; ++var)

#define PORT_0   0
#define PORT_1   1
#define PORT_2   2
#define PORT_3   3
#define PORT_4   4
#define PORT_5   5
#define PORT_6   6
#define PORT_7   7

#define BIT_READ(var, offset) 			((var & (1 << offset)) ? true : false)
#define BIT_WRITE(var, offset, value) 	(value) ? (var |= (1 << offset)) : (var &= ~(1 << offset))

#define WIPE(array, len) foreach(i, 0, len) array[i] = 0;

//-----------------------------------------------------------------------------
// Sys API
//-----------------------------------------------------------------------------

// System reset
void sys_reset();

// Application init
void app_init();
void app_main();

void sys_reset(void);

//-----------------------------------------------------------------------------
// I/O
//-----------------------------------------------------------------------------


extern byte data *display;
extern byte data *dimmer;
extern byte       keyboard[4];

extern uint16     timer;
extern uint16     locker;

void swap_display();
void setup_io();

void flash(byte count);

#endif