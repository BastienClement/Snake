#include "sys.h"

//-----------------------------------------------------------------------------
// System init
//-----------------------------------------------------------------------------

byte i, j, k;

// Clock init
void SYSCLK_init(void) {
	OSCXCN = 0x67;				// demarrage de l' oscillateur externe(22.1184MHz ) 
	
	for(i=0; i < 255; i++); 	// Attente le démarrage de l'oscillateur
	while(!(OSCXCN & 0x80)); 	// Attendre la stabilisation de l'oscillateur
		
	OSCICN = 0x88; 				// Selectionner l'oscillateur ext. pour SYSCLK
								// et activer le detecteur d'erreur d'oscillation.
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

void main(void) {
	// Disable watchdog timer
	WDTCN = 0xDE;
	WDTCN = 0xAD;
	
	// Clock init
	SYSCLK_init();
	
	// XBARE: enable crossbar
	XBR2 |= (BIT6);
	
	// Configure and init application
	app_init();
}

void sys_reset(void) {
	((void (code *)(void)) 0x0000)();
}
