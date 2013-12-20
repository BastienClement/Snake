#include "sys.h"

// Les directions du serpent
typedef enum {
	UP,
	RIGHT,
	DOWN,
	LEFT
} dir;

// États du jeu
pdata byte snake[48];			// Trace du serpent
byte snake_len      = 4;		// Longueur actuelle du serpent
dir  snake_last_dir = RIGHT;	// Direction du dernier déplacement
dir  snake_next_dir = RIGHT;	// Direction du prochain déplacement
byte score          = 0;		// Score
byte fruit          = 0xFF;		// Coordonnées du fruit
bool pause          = true;		// Pause activée
bool playing        = true;		// Le jeu est en cours
bool wall_enabled   = false;	// Les murs sont actifs

// Interval de temps entre les déplacements (en ms)
uint16 game_tick_interval = 400;

// Affiche un chiffre sur la matrice
// (à l'exception des points commun à tous les chiffres)
// * Si 'shift', le chiffre correspond aux unités
// * Sinon, le chiffre correspond aux dizaines
#define DRAW(line, col, cond) if(cond) display[line] |= (1 << col) << (shift ? 0 : 4)
#define D(n) (digit == n)
void draw_digit(byte digit, bool shift) {
	DRAW(1, 1, ! D(1)                    );
	DRAW(1, 2, !(D(1)||D(4))             );
	DRAW(2, 1, !(D(1)||D(2)||D(3)||D(7)) );
	DRAW(2, 3, !(D(5)||D(6))             );
	DRAW(3, 1, !(D(1)||D(7))             );
	DRAW(3, 2, !(D(0)||D(1)||D(7))       );
	DRAW(4, 1,  (D(0)||D(2)||D(6)||D(8)) );
	DRAW(4, 3, ! D(2)                    );
	DRAW(5, 1, !(D(1)||D(4)||D(7)||D(9)) );
	DRAW(5, 2, !(D(1)||D(4)||D(7)||D(9)) );
}

// Affiche le score
void display_score() {
	byte tens, units;
	
	// Séparation unités / dizaines
	tens  = score / 10;
	units = score - (tens * 10);
	
	// Dessin des points commun à tous les chiffres
	display[1] = display[3] = display[5] = 0x88;
	
	// Dessin des unités et dizaines
	draw_digit(tens, true);
	draw_digit(units, false);
	
	// Affichage
	swap_display();
}

// Crée un nouveau fruit et s'assure qu'il n'est pas dans le serpent
void create_fruit() {
	bool valid;
	
	do {
		// Nouveau fruit, à priori valide
		fruit = rand() & 0x77;
		valid = true;
		
		// Parcours du serpent à la recherche de collision
		foreach(i, 0, snake_len) {
			if(snake[i] == fruit) {
				// Le fruit est dans le serpent
				valid = false;
				break;
			}
		}
		
		// On essaie une nouvelle fois !
	} while(!valid);
}

// Un tick du jeu, correspond à l'avancée du serpent
void game_tick() {
	// Coordonnées de la nouvelle tête
	byte head_x, head_y, new_head;
	
	// Extraction des coordonnées actuelles
	head_x = (snake[0] & 0xF0) >> 4;
	head_y = snake[0] & 0x0F;
	
	// Déplacement de la tête
	#define HEAD_MOVE(cond, yes, no) \
		if(cond) { \
			if(wall_enabled) { \
				playing = false; \
			} else { \
				no; \
			} \
		} else { \
			yes; \
		} \
		break;
	
	switch(snake_next_dir) {
		case UP:	HEAD_MOVE(head_y < 1, --head_y, head_y = 7);
		case RIGHT: HEAD_MOVE(head_x > 6, ++head_x, head_x = 0);
		case DOWN:  HEAD_MOVE(head_y > 6, ++head_y, head_y = 0);
		case LEFT:  HEAD_MOVE(head_x < 1, --head_x, head_x = 7);
	}
	
	// Mise à jour de la direction
	snake_last_dir = snake_next_dir;
	
	// Construction du byte de coordonées
	new_head = (head_x << 4) | head_y;
	
	// Si la nouvelle tête est sur le fruit
	if(new_head == fruit) {
		// Incrémentation du score
		if(score < 99) {
			++score;
		}
		
		// Ajout d'un segement au serpent
		if(snake_len < (sizeof(snake)-1)) {
			++snake_len;
		}
		
		// Accélération du jeu
		game_tick_interval = (game_tick_interval * 19) / 20;
		if(game_tick_interval < 200) game_tick_interval = 200;
	}
	
	// Avancement du serpent
	for(i = (snake_len - 1); i > 0; --i) {
		snake[i] = snake[i-1];
	}
	
	// Ajout de la nouvelle tête
	snake[0] = new_head;
	
	// Si on vient de manger le fruit, on en place un nouveau
	if(new_head == fruit) {
		create_fruit();
	}
}

// Initialisation du RNG
void init_rng() {
	srand(TH1 ^ TL1);
}

// Initialisation du jeu et boucle principale
void app_init() {
	bool repaint    = true;		// Repaint nécessaire
	bool pause_key  = false;	// La touche pause a été activée
	bool step_key   = false;	// La touche step a été activée
	bool wall_key   = false;	// La touche wall a été activée
	bool breakpoint = false;	// Breakpoint flag
	
	// Activation des entrées sorties
	setup_io();
	
	// Position initiale du serpent
	snake[0] = 0x43;
	snake[1] = 0x33;
	snake[2] = 0x23;
	snake[3] = 0x13;
	
	// Boucle principale
	while(playing) {
		// --- Keyboard ---
		// Touche directionnelles
		if(keyboard[0] & 0x02 && snake_last_dir != DOWN) {
			snake_next_dir = UP;
		} else if(keyboard[1] & 0x01 && snake_last_dir != RIGHT) {
			snake_next_dir = LEFT;
		} else if(keyboard[1] & 0x04 && snake_last_dir != LEFT) {
			snake_next_dir = RIGHT;
		} else if(keyboard[1] & 0x02 && snake_last_dir != UP) {
			snake_next_dir = DOWN;
		}
		
		// Play / pause
		if(keyboard[0] & 0x08) {
			pause_key = true;
		}
		
		// Step
		if(keyboard[1] & 0x08) {
			step_key = true;
		} else if(step_key) {
			pause_key = false;
			pause = false;
			step_key = false;
			breakpoint = true;
			init_rng();
		}
		
		// Walls
		if(keyboard[3] & 0x01) {
			wall_key = true;
		} else if(wall_key) {
			wall_key = false;
			wall_enabled = !wall_enabled;
			repaint = true;
			flash(0);
		}
		
		// --- Game progress ---
		// Attente du temps minimum avant déplacement
		if(timer > game_tick_interval) {
			timer = 0;
			
			// Gestion de la touche pause
			if(pause_key && !(keyboard[0] & 0x08)) {
				pause_key = false;
				pause = !pause;
				
				// Initialisation du RNG
				init_rng();
			}
			
			// Avancée du serpent
			if(!pause) {
				// Création du premier fruit
				if(fruit == 0xFF) {
					create_fruit();
				}
				
				game_tick();
			}
			
			repaint = true;
		
			if(breakpoint) {
				pause = true;
				breakpoint = false;
			}
		}
		
		// --- Display ---
		if(repaint) {
			// Snake
			foreach(i, 0, snake_len) {
				// Coordonnées de la section actuelle du serpent
				byte coords = snake[i];
				
				// On vérifie sur cet emplacement est déjà occupé
				// (le serpent se mange lui-même)
				if(display[coords & 0x0F] & (1 << ((coords & 0xF0) >> 4))) {
					playing = false;
				}
				
				// Dessin du serpent sur l'affichage
				display[coords & 0x0F] |= (1 << ((coords & 0xF0) >> 4));
			}
			
			// Fruit
			display[fruit & 0x0F] |= (1 << ((fruit & 0xF0) >> 4));
			dimmer[fruit & 0x0F] |= (1 << ((fruit & 0xF0) >> 4));
			
			// Swap display buffers
			swap_display();
			
			// Affichage du score sur les LEDs
			P0 = score;
		}
	}
	
	flash(5);
	timer = 0;
	while(timer < 1000);
	
	// --- Score ---
	display_score();
	
	// Wait for reset
	while(1) {
		if(keyboard[3] & 0x08) {
			sys_reset();
		}
	};
}
