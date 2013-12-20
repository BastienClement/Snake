#include "sys.h"

// Mémoire d'affichage
byte display_buffer0[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
byte display_buffer1[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

byte data *display = &display_buffer1;
byte data *front_display = &display_buffer0;
bit current_display = 0;

byte display_dimmer0[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
byte display_dimmer1[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

byte data *dimmer = &display_dimmer1;
byte data *front_dimmer = &display_dimmer0;

// Mémoire d'entrée
byte keyboard[4] = { 0x00, 0x00, 0x00, 0x00 };

// Compteur temps réel (incrémenté de 1 chaque ms)
uint16 timer  = 0;
uint16 locker = 0;

byte fx_flash = 0;
bool fx_flag  = true;

//
// --- Gestion d'E/S ---
//

// Alterne les deux buffers graphiques
void swap_display() {
	if(current_display) {
		front_display = &display_buffer0;
		display = &display_buffer1;
		
		front_dimmer = &display_dimmer0;
		dimmer = &display_dimmer1;
	} else {
		front_display = &display_buffer1;
		display = &display_buffer0;
		
		front_dimmer = &display_dimmer1;
		dimmer = &display_dimmer0;
	}
	
	current_display = !current_display;
	WIPE(display, 8);
	WIPE(dimmer, 8);
}

// Contrôle le monostable de la matrice d'affichage
sbit display_trigger = P2 ^ 3;

// Interruption gérant les entrées / sorties
void io_interrupt() interrupt 1 {
	// État actuel des différents périphériques
	static bool z_flag = true;
	static byte display_row = 0;
	static byte keyboard_row = 255;
	static byte dimmer = 0;
	
	// La prochaine interruption sera déclanchée 0.5ms
	TH0 = (-SYSCLK/2000) >> 8;
	TL0 = -SYSCLK/2000;
	
	z_flag = !z_flag;
	
	// --- Timer ---
	// Incrémentation du compteur de temps
	if(z_flag) {
		timer++;
		locker++;
	}
	
	// --- Display ---
	// Sélection de la ligne suivante
	if(++display_row > 7) {
		display_row = 0;
		if(++dimmer > 3) {
			dimmer = 0;
		}
	}
	
	// Envoi de la ligne
	P1 = (fx_flash ? ~front_display[display_row] : front_display[display_row]) & (dimmer ? ~front_dimmer[display_row] : 0xFF);
	
	P2 = display_row;
	display_trigger = 1;
	
	// Effet flash
	if(fx_flash) {
		if(z_flag) {
			--fx_flash;
		}
	} else {
		fx_flag = true;
	}
	
	// --- Keyboard ---
	// Capture de la ligne sélectionnée lors du dernier tick,
	// si appplicable.
	if(keyboard_row < 4) {
		keyboard[keyboard_row] = (~P7) >> 4;
	}
	
	// Sélection de la ligne suivante en prévision
	// de la prochaine interruption
	if(++keyboard_row > 3) keyboard_row = 0;
	switch(keyboard_row) {
		case 0: P7 = 0xFE; break;
		case 1: P7 = 0xFD; break;
		case 2: P7 = 0xFB; break;
		case 3: P7 = 0xF7; break;
	}
}

// Effectue un effet de flash sur l'écran
void flash(byte count) {
	if(count) {
		for(i = 0; i < count; i++) {
			fx_flash = 100;
			fx_flag = false;
			while(!fx_flag);
			timer = 0;
			while(timer < 200);
		}
	} else {
		fx_flash = 100;
		fx_flag = false;
	}
}

// Configure le système pour les entrées / sorties
void setup_io() {
	// Configuration des ports de sortie
	P1MDOUT  = 0xFF;		// Display Data
	P2MDOUT  = 0x0F;		// Display Ctrl
	P74OUT  |= 0x40;		// Keyboard Ctrl
	
	// Configuration du timer d'E/S et d'aléatoire
	TMOD = 0x11;	// Configuration des timers en 16 bits
	ET0 = 1;		// Activation des interruptions pour le timer 0
	TR0 = TR1 = 1;	// Activation des timers
	CKCON |= 0x18;	// Utilisation de la clock system pour les timers
	EA = 1;			// Activation générale des interruptions
}
