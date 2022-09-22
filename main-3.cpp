

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "Random.h"
#include "PLL.h"
#include "SlidePot.h"
#include "Images.h"
#include "UART.h"
#include "Timer0.h"
#include "Timer1.h"
#include "Sound.h"

SlidePot Sensor(1636, 191);
int level;
int song;

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void SysTick_Handler(void);

#define r 0
#define Y 1
#define B 2
#define R 3
#define G 4
#define BY 5
#define GY 6
#define RY 7


//array for the up song button sequence
int upSeq[83] = {R,G,R,BY,BY,BY,BY,BY,B,B,B,R,G,R,BY,BY,GY,RY,
									BY,BY,BY,BY,R,GY,GY,R,R,R,B,GY,GY,R,R,B,B,Y,Y,Y,Y,BY,B,
									B,R,G,R,B,B,Y,Y,Y,B,B,R,G,R,R,R,R,G,R,B,B,r,r,r,Y,Y,Y,Y,B,B,B,B,R,G,r,r,r,r,r,r,r,r};

int twinkleSeq[75] = {Y,Y,r,Y,Y,r,B,B,r,B,B,r,R,R,r,R,R,r,B,B,B,B,r,B,B,r,B,B,r,Y,Y,r,Y,Y,r,G,G,r,R,R,r,B,B,r,r,r,r,Y,Y,r,B,B,r,G,G,r,R,R,r,B,B,r,Y,Y,r,Y,Y,r,r,r,r,r,r,r,r};

// Creating a class for the Sprite.

class yellow {
public:
	int y;
	int x;
	int status; //1 for used, 0 for free
	char block;
	
	yellow(int status, char block) {
		this -> y = 0; //0
		this -> x = 4; //8
		this -> status = status;
		this -> block = block; //n for note, s for solid
	}
	
	void move(void) {
		this -> y = y + 24;
	}
	
	void display(void) {
		if (block == 'n') {
			ST7735_DrawBitmap(x, y, yellownote, 24, 24);
		}
		else if (block == 's') {
			ST7735_DrawBitmap(x, y, yellowsquare, 24, 24);
		}
	}
	
	void erase(void) {
		ST7735_DrawBitmap(x, y, blackall, 24, 24);
		//ST7735_FillRect(x, y - 23, 24, 24, 0);
	}
	
	void reset(void) {
		x = 4;
		y = 0;
		status = 0;
	}
		
};

class blue {
public:
	int y;
	int x;
	int status; //1 for used, 0 for free
	char block;
	
	blue(int status, char block) {
		this -> y = 0; //0
		this -> x = 36; //8
		this -> status = status;
		this -> block = block; //n for note, s for solid
	}
	
	void move(void) {
		this -> y = y + 24;
	}
	
	void display(void) {
		if (block == 'n') {
			ST7735_DrawBitmap(x, y, bluenote, 24, 24);
			//ST7735_DrawBitmap(x, y, bluenote, 12, 18);
		}
		else if (block == 's') {
			ST7735_DrawBitmap(x, y, bluesquare, 24, 24);
		}
	}
	
	void erase(void) {
		//ST7735_DrawBitmap(x, y, blackall, 24, 24);
		ST7735_FillRect(x, y - 23, 24, 24, 0);
		//ST7735_FillRect(x, y - 30, 24, 31, 0);
	}
	
	void reset(void) {
		x = 36;
		y = 0;
		status = 0;
	}
		
};

class red {
public:
	int y;
	int x;
	int status; //1 for used, 0 for free
	char block;
	
	red(int status, char block) {
		this -> y = 0; //0
		this -> x = 64; //8
		this -> status = status;
		this -> block = block; //n for note, s for solid
	}
	
	void move(void) {
		this -> y = y + 24;
	}
	
	void display(void) {
		if (block == 'n') {
			ST7735_DrawBitmap(x, y, rednote, 24, 24);
		}
		else if (block == 's') {
			ST7735_DrawBitmap(x, y, redsquare, 24, 24);
		}
	}
	
	void erase(void) {
		ST7735_DrawBitmap(x, y, blackall, 24, 24);
		//ST7735_FillRect(x, y - 23, 24, 24, 0);
	}
	
	void reset(void) {
		x = 64;
		y = 0;
		status = 0;
	}
		
};
class green {
public:
	int y;
	int x;
	int status; //1 for used, 0 for free
	char block;
	
	green(int status, char block) {
		this -> y = 0; //0
		this -> x = 92; //8
		this -> status = status;
		this -> block = block; //n for note, s for solid
	}
	
	void move(void) {
		this -> y = y + 24;
	}
	
	void display(void) {
		if (block == 'n') {
			ST7735_DrawBitmap(x, y, greennote, 24, 24);
		}
		else if (block == 's') {
			ST7735_DrawBitmap(x, y, greensquare, 24, 24);
		}
	}
	
	void erase(void) {
		ST7735_DrawBitmap(x, y, blackall, 24, 24);
		//ST7735_FillRect(x, y - 23, 24, 24, 0);
	}
	
	void reset(void) {
		x = 92;
		y = 0;
		status = 0;
	}
		
};


yellow y1(0, 'n');
yellow y2(0, 'n');
yellow y3(0, 'n');
yellow y4(0, 'n');
yellow y5(0, 'n');
yellow y6(0, 'n');
yellow y7(0, 'n');
yellow y8(0, 'n');

blue b1(0, 'n');
blue b2(0, 'n');
blue b3(0, 'n');
blue b4(0, 'n');
blue b5(0, 'n');
blue b6(0, 'n');
blue b7(0, 'n');
blue b8(0, 'n');

red r1(0, 'n');
red r2(0, 'n');
red r3(0, 'n');
red r4(0, 'n');
red r5(0, 'n');
red r6(0, 'n');
red r7(0, 'n');
red r8(0, 'n');

green g1(0, 'n');
green g2(0, 'n');
green g3(0, 'n');
green g4(0, 'n');
green g5(0, 'n');
green g6(0, 'n');
green g7(0, 'n');
green g8(0, 'n');

int yellowInd = 0; //0 for no 1 for yes
int blueInd = 0;
int redInd = 0;
int greenInd = 0;

yellow yellows[8] = {y1, y2, y3, y4, y5, y6, y7, y8};
blue blues[8] = {b1, b2, b3, b4, b5, b6, b7, b8};
red reds[8] = {r1, r2, r3, r4, r5, r6, r7, r8};
green greens[8] = {g1, g2, g3, g4, g5, g6, g7, g8};

int UpIndex; 
int yellowindex;
int previouscolor; //previous color for note or solid


void GameEngine(void) {	
	static int counter = 0;
	counter++;
	if (counter % 24 == 0) {
		if (song == 0) {
			if (upSeq[UpIndex] == Y || upSeq[UpIndex] == BY || upSeq[UpIndex] == GY || upSeq[UpIndex] == RY) { 
				//if any combination of yellow
				int i;
				for (i = 0; i < 8; i++) {
					if (yellows[i].status == 0) {
						break;
					}
				}
				yellows[i].status = 1; //status is now used
				if (previouscolor == Y || previouscolor == BY || previouscolor == GY || previouscolor == RY) {
					yellows[i].block = 's';
				}
				else {
					yellows[i].block = 'n';
				}
				previouscolor = upSeq[UpIndex]; //update previous color when done
			}
			
			if (upSeq[UpIndex] == BY || upSeq[UpIndex] == B) { 
				//if any combination of yellow
				int i;
				for (i = 0; i < 8; i++) {
					if (blues[i].status == 0) {
						break;
					}
				}
				blues[i].status = 1; //status is now used
				if (previouscolor == B || previouscolor == BY) {
					blues[i].block = 's';
				}
				else {
					blues[i].block = 'n';
				}
				previouscolor = upSeq[UpIndex]; //update previous color when done
			}
			
			if (upSeq[UpIndex] == R || upSeq[UpIndex] == RY) { 
				//if any combination of red
				int i;
				for (i = 0; i < 8; i++) {
					if (reds[i].status == 0) {
						break;
					}
				}
				reds[i].status = 1; //status is now used
				if (previouscolor == R || previouscolor == RY) {
					reds[i].block = 's';
				}
				else {
					reds[i].block = 'n';
				}
				previouscolor = upSeq[UpIndex]; //update previous color when done
			}
			
			if (upSeq[UpIndex] == G || upSeq[UpIndex] == GY) { 
				//if any combination of green
				int i;
					for (i = 0; i < 8; i++) {
						if (greens[i].status == 0) {
							break;
						}
					}
					greens[i].status = 1; //status is now used
					if (previouscolor == G || previouscolor == GY) {
						greens[i].block = 's';
					}
					else {
						greens[i].block = 'n';
					}
					previouscolor = upSeq[UpIndex]; //update previous color when done
				}
				UpIndex++;
			}
		else {
			if (twinkleSeq[UpIndex] == Y || twinkleSeq[UpIndex] == BY || twinkleSeq[UpIndex] == GY || twinkleSeq[UpIndex] == RY) { 
				//if any combination of yellow
				int i;
				for (i = 0; i < 8; i++) {
					if (yellows[i].status == 0) {
						break;
					}
				}
				yellows[i].status = 1; //status is now used
				if (previouscolor == Y || previouscolor == BY || previouscolor == GY || previouscolor == RY) {
					yellows[i].block = 's';
				}
				else {
					yellows[i].block = 'n';
				}
				previouscolor = twinkleSeq[UpIndex]; //update previous color when done
			}
			
			if (twinkleSeq[UpIndex] == BY || twinkleSeq[UpIndex] == B) { 
				//if any combination of yellow
				int i;
				for (i = 0; i < 8; i++) {
					if (blues[i].status == 0) {
						break;
					}
				}
				blues[i].status = 1; //status is now used
				if (previouscolor == B || previouscolor == BY) {
					blues[i].block = 's';
				}
				else {
					blues[i].block = 'n';
				}
				previouscolor = twinkleSeq[UpIndex]; //update previous color when done
			}
			
			if (twinkleSeq[UpIndex] == R || twinkleSeq[UpIndex] == RY) { 
				//if any combination of red
				int i;
				for (i = 0; i < 8; i++) {
					if (reds[i].status == 0) {
						break;
					}
				}
				reds[i].status = 1; //status is now used
				if (previouscolor == R || previouscolor == RY) {
					reds[i].block = 's';
				}
				else {
					reds[i].block = 'n';
				}
				previouscolor = twinkleSeq[UpIndex]; //update previous color when done
			}
			
			if (twinkleSeq[UpIndex] == G || twinkleSeq[UpIndex] == GY) { 
				//if any combination of green
				int i;
					for (i = 0; i < 8; i++) {
						if (greens[i].status == 0) {
							break;
						}
					}
					greens[i].status = 1; //status is now used
					if (previouscolor == G || previouscolor == GY) {
						greens[i].block = 's';
					}
					else {
						greens[i].block = 'n';
					}
					previouscolor = twinkleSeq[UpIndex]; //update previous color when done
				}
				UpIndex++;
			}
		}
		if (counter % 24 == 0) {
			for (int i = 0; i < 8; i++) {
				if (yellows[i].status == 1) {
					yellows[i].erase();
					yellows[i].move();
					yellows[i].display();
					if (yellows[i].y > 100) {
						yellowInd = 1;
					}
					if (yellows[i].y >= 160) {
						yellows[i].reset();
						yellowInd = 0;
						ST7735_DrawBitmap(4, 160, blackall, 24, 24);
					}
				}
				
				if (blues[i].status == 1) {
					blues[i].erase();
					blues[i].move();
					blues[i].display();
					if (blues[i].y > 100) {
						blueInd = 1;
					}
					if (blues[i].y >= 160) {
						blues[i].reset();
						blueInd = 0;
						ST7735_DrawBitmap(36, 160, blackall, 24, 24);
					}
				}
				
				if (reds[i].status == 1) {
					reds[i].erase();
					reds[i].move();
					reds[i].display();
					if (reds[i].y > 100) {
						redInd = 1;
					}
					if (reds[i].y >= 160) {
						reds[i].reset();
						redInd = 0;
						ST7735_DrawBitmap(64, 160, blackall, 24, 24);
					}
				}
				
					if (greens[i].status == 1) {
					greens[i].erase();
					greens[i].move();
					greens[i].display();
					if (greens[i].y > 100) {
						greenInd = 1;
					}
					if (greens[i].y >= 160) {
						greens[i].reset();
						greenInd = 0;
						ST7735_DrawBitmap(92, 160, blackall, 24, 24);
					}
				}
			}
		}	

}


void gameTest(void) {
	static int x = 8;
	static int y = 0;
	ST7735_FillRect(x, y, 24, 24, 0);
	y += 8;
	ST7735_FillRect(x, y, 24, 24, 0xFFFF);
	
}

void Switch_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x00000010; // activate clock for port E
	while((SYSCTL_PRGPIO_R & 0x00000010) == 0){}; //wait for initialization
	
	GPIO_PORTE_AMSEL_R &= ~0x0F;
	GPIO_PORTE_DIR_R  &= ~0x0F;
	GPIO_PORTE_DEN_R  |=  0x0F;
//	GPIO_PORTE_PCTL_R &= ~0x0000FFFF;
	GPIO_PORTE_AFSEL_R &= ~0x0F;
	GPIO_PORTE_IS_R   &= ~0x0F;
	GPIO_PORTE_IBE_R &= ~0x0F;    //     PF4 is not both edges
	GPIO_PORTE_IEV_R |= 0x0F;    //     PF4 falling edge event
	GPIO_PORTE_ICR_R = 0x0F;      // (e) clear flag4
	GPIO_PORTE_IM_R |= 0x0F;      // (f) arm interrupt 
//	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF1F)|0x00000020;
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF1F)|0x00000020; // (g) priority 1
	NVIC_EN0_R |= 0x00000010;      // (h) enable interrupt 4 in NVIC
	//EnableInterrupts();           // (i) Enable global Interrupt flag (I)

}

int lang; //0 for english, 1 for spanish

void language() {
	//  ST7735_FillScreen(0x0000);            // set screen to black
//  ST7735_SetCursor(1, 1);
//  ST7735_OutString((char*)"GAME OVER");
//  ST7735_SetCursor(1, 2);
//  ST7735_SetTextColor(ST7735_WHITE);
//  ST7735_OutString((char*)"Nice try,");
//  ST7735_SetCursor(1, 3);
//  ST7735_OutString((char*)"Earthling!");
//  ST7735_SetCursor(2, 4);
//  ST7735_SetTextColor(ST7735_WHITE);
//  while(1){
//    while(flag==0){};
//    flag = 0;
//    ST7735_SetCursor(2, 4);
//    ST7735_OutUDec(time);
//  }
	ST7735_FillScreen(0x0000);
	ST7735_SetCursor(1, 1);
	ST7735_OutString((char*) "Piano Tiles!");
	ST7735_SetCursor(3, 3);
	ST7735_OutString((char*) "Press Yellow");
	ST7735_SetCursor(3, 4);
	ST7735_OutString((char*) "for English");
	ST7735_SetCursor(3, 6);
	ST7735_OutString((char*) "Presione Azul");
	ST7735_SetCursor(3, 7);
	ST7735_OutString((char*) "para Espa\xA4ol");
	
	while ((GPIO_PORTE_DATA_R & 0x01) == 0 && (GPIO_PORTE_DATA_R & 0x02) == 0) {}
	
	int delay = 100000;
	while (delay!=0) {
		delay--;
	}
	
	if ((GPIO_PORTE_DATA_R & 0x01) == 1) {
		lang = 0;
	}
	else {
		lang = 1;
	}
}


int score;
extern "C" void GPIOE_Handler(void);
void GPIOE_Handler(void) {
	GPIO_PORTE_ICR_R = 0x0F;
	if ((GPIO_PORTE_DATA_R & 0x01) == 1) {
		if (yellowInd == 1) {
			score++;
			yellowInd = 0;
		}
	}
	if ((GPIO_PORTE_DATA_R & 0x02) == 1) {
		if (blueInd == 1) {
			score++;
			blueInd = 0;
		}
	}
	if ((GPIO_PORTE_DATA_R & 0x04) == 1) {
		if (redInd == 1) {
			score++;
			redInd = 0;
		}
	}
	if ((GPIO_PORTE_DATA_R & 0x08) == 1) {
		if (greenInd == 1) {
			score++;
			greenInd = 0;
		}
	}
	
}

void SysTick_Init(uint32_t period){
  //*** students write this ******
	NVIC_ST_RELOAD_R = period;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x07;
}

void SysTick_off(void) {
	NVIC_ST_CTRL_R = 0x00;
}

void SysTick_Handler(void){ // every sample
    //*** students write this ******
	int32_t n = ADC_In();
	if (n < 1300) {
		level = 1;
	}
	else if (n > 1300 && n < 2600) {
		level = 2;
	}
	else {
		level = 3;
	}
}

void menu(void) {
	if (lang == 0) { //english
		ST7735_FillScreen(0x0000);
		ST7735_SetCursor(1, 1);
		ST7735_OutString((char*) "Similar to the game");
		ST7735_SetCursor(1, 2);
		ST7735_OutString((char*) "Guitar Hero, press");
		ST7735_SetCursor(1, 3);
		ST7735_OutString((char*) "the colored buttons");
		ST7735_SetCursor(1, 4);
		ST7735_OutString((char*) "when corresponding");
		ST7735_SetCursor(1, 5);
		ST7735_OutString((char*) "colored tiles reach");
		ST7735_SetCursor(1, 6);
		ST7735_OutString((char*) "the bottom of the");
		ST7735_SetCursor(1, 7);
		ST7735_OutString((char*) "screen. Press");
		ST7735_SetCursor(1, 8);
		ST7735_OutString((char*) "red to continue!");
		
		yellow ymenu(0, 'n');
		ymenu.x = 4;
		ymenu.y = 135;
		blue bmenu(0, 'n');
		bmenu.x = 36;
		bmenu.y = 135;
		red rmenu(0, 'n');
		rmenu.x = 68;
		rmenu.y = 135;
		green gmenu(0, 'n');
		gmenu.x = 100;
		gmenu.y = 135;
		
		ymenu.display();
		bmenu.display();
		rmenu.display();
		gmenu.display();
		
		while ((GPIO_PORTE_DATA_R & 0x04) == 0){}
		
		int delay = 100000;
		while (delay!=0) {
			delay--;
		}
			
		ST7735_FillScreen(0x0000);
		ST7735_SetCursor(1, 1);
		ST7735_OutString((char*) "Adjust slider for");
		ST7735_SetCursor(1, 2);
		ST7735_OutString((char*) "score multipliers.");
		ST7735_SetCursor(1, 3);
		ST7735_OutString((char*) "Easy(1x)");
		ST7735_SetCursor(1, 4);
		ST7735_OutString((char*) "Medium(2x)");
		ST7735_SetCursor(1, 5);
		ST7735_OutString((char*) "Hard(3x)");
		ST7735_SetCursor(1, 6);
		ST7735_OutString((char*) "Adjust before");
		ST7735_SetCursor(1, 7);
		ST7735_OutString((char*) "pressing green");
		ST7735_SetCursor(1, 8);
		ST7735_OutString((char*) "button.");
		
		
		while ((GPIO_PORTE_DATA_R & 0x08) == 0){}
		
		delay = 100000;
		while (delay!=0) {
			delay--;
		}
				
		
		ST7735_FillScreen(0x0000);
		ST7735_SetCursor(1, 1);
		ST7735_OutString((char*) "Difficulty Selected");
		ST7735_SetCursor(1, 2);
		if (level == 1){
			ST7735_OutString((char*) "Easy!");
		}
		else if (level == 2) {
			ST7735_OutString((char*) "Medium!");
		}
		else {
			ST7735_OutString((char*) "Hard!");
		}
		ST7735_SetCursor(1, 4);
		ST7735_OutString((char*) "Blue for Twinkle");
		ST7735_SetCursor(1, 5);
		ST7735_OutString((char*) "Twinkle");
		ST7735_SetCursor(1, 7);
		ST7735_OutString((char*) "Red for Married");
		ST7735_SetCursor(1, 8);
		ST7735_OutString((char*) "Life");
		
		while ((GPIO_PORTE_DATA_R & 0x06) == 0) {}
		
		if ((GPIO_PORTE_DATA_R & 0x04) == 0x04) {
				song = 0;
				ST7735_FillScreen(0x0000);
				Sound_Init();
				UpIndex = 0;
				yellowindex = 0;
				previouscolor = -1;
				SysTick_off();
			if (level == 1) {
				changeFreq(1);
				Timer0_Init(&GameEngine, 1420000); //1050000 for medium, 1420000 for slow, 797000 for fast
				int x = 50000000; //40000000 for medium, 5 for slow, 
				while (x != 0){
					x--;
				}
				soundPlay(0);
			}
			else if (level == 2) {
				changeFreq(2);
				Timer0_Init(&GameEngine, 1050000); //1050000 for medium, 1420000 for slow, 797000 for fast
				int x = 40000000; //40000000 for medium, 5 for slow, 
				while (x != 0){
					x--;
				}
				soundPlay(0);
			}
			else {
				changeFreq(3);
				Timer0_Init(&GameEngine, 797000); //1050000 for medium, 1420000 for slow, 797000 for fast
				int x = 25000000; //40000000 for medium, 5 for slow, 
				while (x != 0){
					x--;
				}
				soundPlay(0);
			}
				
		}
		
		if ((GPIO_PORTE_DATA_R & 0x02) == 0x02) {
				song = 1;
				ST7735_FillScreen(0x0000);
				Sound_Init();
				UpIndex = 0;
				yellowindex = 0;
				previouscolor = -1;
				SysTick_off();
			if (level == 1) {
				changeFreq(1);
				Timer0_Init(&GameEngine, 1460000); //1050000 for medium, 1420000 for slow, 797000 for fast
				int x = 55000000; //40000000 for medium, 5 for slow, 
				while (x != 0){
					x--;
				}
				soundPlay(1);
			}
			else if (level == 2) {
				changeFreq(2);
				Timer0_Init(&GameEngine, 1100000); //1050000 for medium, 1420000 for slow, 797000 for fast
				int x = 36000000; //40000000 for medium, 5 for slow, 
				while (x != 0){
					x--;
				}
				soundPlay(1);
			}
			else {
				changeFreq(3);
				Timer0_Init(&GameEngine, 850000); //1050000 for medium, 1420000 for slow, 797000 for fast
				int x = 26000000; //40000000 for medium, 5 for slow, 
				while (x != 0){
					x--;
				}
				soundPlay(1);
			}
				
		}
		
}
	else { //spanish
		ST7735_FillScreen(0x0000);
		ST7735_SetCursor(1, 1);
		ST7735_OutString((char*) "Parecido al juego");
		ST7735_SetCursor(1, 2);
		ST7735_OutString((char*) "H\x82roe de la");
		ST7735_SetCursor(1, 3);
		ST7735_OutString((char*) "Guitarra, presiona");
		ST7735_SetCursor(1, 4);
		ST7735_OutString((char*) "los botones de");
		ST7735_SetCursor(1, 5);
		ST7735_OutString((char*) "colores cuando");
		ST7735_SetCursor(1, 6);
		ST7735_OutString((char*) "corresponde tejas");
		ST7735_SetCursor(1, 7);
		ST7735_OutString((char*) "decolores alcanzan");
		ST7735_SetCursor(1, 8);
		ST7735_OutString((char*) "la parte inferior");
		ST7735_SetCursor(1, 9);
		ST7735_OutString((char*) "de la pantalla. ");
		ST7735_SetCursor(1, 10);
		ST7735_OutString((char*) "Presione rojo");
		ST7735_SetCursor(1, 11);
		ST7735_OutString((char*) "para continuar!");
		 
		
		
		yellow ymenu(0, 'n');
		ymenu.x = 4;
		ymenu.y = 145;
		blue bmenu(0, 'n');
		bmenu.x = 36;
		bmenu.y = 145;
		red rmenu(0, 'n');
		rmenu.x = 68;
		rmenu.y = 145;
		green gmenu(0, 'n');
		gmenu.x = 100;
		gmenu.y = 145;
		
		ymenu.display();
		bmenu.display();
		rmenu.display();
		gmenu.display();
		
		while ((GPIO_PORTE_DATA_R & 0x04) == 0){}
		
		ST7735_FillScreen(0x0000);
		ST7735_SetCursor(1, 1);
		ST7735_OutString((char*) "Adjustar control");
		ST7735_SetCursor(1, 2);
		ST7735_OutString((char*) "deslizante para");
		ST7735_SetCursor(1, 3);
		ST7735_OutString((char*) "multiplicadores");
		ST7735_SetCursor(1, 4);
		ST7735_OutString((char*) "de puntuacion");
		ST7735_SetCursor(1, 5);
		ST7735_OutString((char*) "Facil(1x)");
		ST7735_SetCursor(1, 6);
		ST7735_OutString((char*) "Medio(2x)");
		ST7735_SetCursor(1, 7);
		ST7735_OutString((char*) "Dificil(3x)");
		ST7735_SetCursor(1, 8);
		ST7735_OutString((char*) "Adjustar antes de");
		ST7735_SetCursor(1, 9);
		ST7735_OutString((char*) "pulsar el buton");
		ST7735_SetCursor(1, 10);
		ST7735_OutString((char*) "verde.");
		
		
		while ((GPIO_PORTE_DATA_R & 0x08) == 0){}
			
		
		ST7735_FillScreen(0x0000);
		ST7735_SetCursor(1, 1);
		ST7735_OutString((char*) "Dificultad");
		ST7735_SetCursor(1, 2);
		ST7735_OutString((char*) "seleccionada");
	  ST7735_SetCursor(1, 3);
		if (level == 1){
			ST7735_OutString((char*) "Facil!");
		}
		else if (level == 2) {
			ST7735_OutString((char*) "Medio!");
		}
		else {
			ST7735_OutString((char*) "Dificil!");
		}
		ST7735_SetCursor(1, 5);
		ST7735_OutString((char*) "Azul para Twinkle");
		ST7735_SetCursor(1, 6);
		ST7735_OutString((char*) "Twinkle");
		ST7735_SetCursor(1, 8);
		ST7735_OutString((char*) "Rojo para Married");
		ST7735_SetCursor(1, 9);
		ST7735_OutString((char*) "Life");
		
		while ((GPIO_PORTE_DATA_R & 0x06) == 0) {}
		
		if ((GPIO_PORTE_DATA_R & 0x04) == 0x04) {
				song = 0;
				ST7735_FillScreen(0x0000);
				Sound_Init();
				UpIndex = 0;
				yellowindex = 0;
				previouscolor = -1;
				SysTick_off();
			if (level == 1) {
				changeFreq(1);
				Timer0_Init(&GameEngine, 1420000); //1050000 for medium, 1420000 for slow, 797000 for fast
				int x = 50000000; //40000000 for medium, 5 for slow, 
				while (x != 0){
					x--;
				}
				soundPlay(0);
			}
			else if (level == 2) {
				changeFreq(2);
				Timer0_Init(&GameEngine, 1050000); //1050000 for medium, 1420000 for slow, 797000 for fast
				int x = 40000000; //40000000 for medium, 5 for slow, 
				while (x != 0){
					x--;
				}
				soundPlay(0);
			}
			else {
				changeFreq(3);
				Timer0_Init(&GameEngine, 797000); //1050000 for medium, 1420000 for slow, 797000 for fast
				int x = 25000000; //40000000 for medium, 5 for slow, 
				while (x != 0){
					x--;
				}
				soundPlay(0);
			}
				
		}
		
		if ((GPIO_PORTE_DATA_R & 0x02) == 0x02) {
				song = 1;
				ST7735_FillScreen(0x0000);
				Sound_Init();
				UpIndex = 0;
				yellowindex = 0;
				previouscolor = -1;
				SysTick_off();
			if (level == 1) {
				changeFreq(1);
				Timer0_Init(&GameEngine, 1460000); //1050000 for medium, 1420000 for slow, 797000 for fast
				int x = 55000000; //40000000 for medium, 5 for slow, 
				while (x != 0){
					x--;
				}
				soundPlay(1);
			}
			else if (level == 2) {
				changeFreq(2);
				Timer0_Init(&GameEngine, 1100000); //1050000 for medium, 1420000 for slow, 797000 for fast
				int x = 36000000; //40000000 for medium, 5 for slow, 
				while (x != 0){
					x--;
				}
				soundPlay(1);
			}
			else {
				changeFreq(3);
				Timer0_Init(&GameEngine, 850000); //1050000 for medium, 1420000 for slow, 797000 for fast
				int x = 26000000; //40000000 for medium, 5 for slow, 
				while (x != 0){
					x--;
				}
				soundPlay(1);
			}
				
		}
	}
}

void outscore(void) {
	ST7735_FillScreen(0x0000);
	ST7735_SetCursor(1, 3);
	if (lang == 0) ST7735_OutString((char*) "Your Score:");
	else ST7735_OutString((char*) "Su Puntuacion:");
	ST7735_SetCursor(1, 4);
	if (level == 2) score *= 2;
	else if (level == 3) score *= 3;
	ST7735_OutUDec(score);
}

int main(void){
  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
	
	DisableInterrupts();
  Output_Init();
	ST7735_SetRotation(2); //rotate 180 degrees
	Switch_Init();
	ADC_Init();
  EnableInterrupts();
	
	SYSCTL_RCGCGPIO_R |= 0x00000010; // activate clock for port E
	while((SYSCTL_PRGPIO_R & 0x00000010) == 0){}; //wait for initialization
	
	GPIO_PORTE_AMSEL_R &= ~0x0F;
	GPIO_PORTE_DIR_R  &= ~0x0F;
	GPIO_PORTE_DEN_R  |=  0x0F;
		
	SysTick_Init(8000000); //100 ms delay
	
	while (1) {
		language();
		menu();
		
		//Sound_Init();
		//UpIndex = 0;
		//yellowindex = 0;
		//previouscolor = -1;
		//Timer0_Init(&GameEngine, 797000); //1050000 for medium, 1420000 for slow, 797000 for fast
		
		if (song == 0) {
			while (UpIndex < 83) {}
			Timer0A_Stop();
		}
		else {
			while (UpIndex < 75) {}
			Timer0A_Stop();
		}
		ST7735_FillScreen(0x0000);
			
		outscore();
		
//		if (lang == 0) {
//			ST7735_SetCursor(1, 6);
//			ST7735_OutString((char*) "Press yellow");
//			ST7735_SetCursor(1, 7);
//			ST7735_OutString((char*) "for new game");
//		}
//		else {
//			ST7735_SetCursor(1, 6);
//			ST7735_OutString((char*) "Presione");
//			ST7735_SetCursor(1, 7);
//			ST7735_OutString((char*) "amarillo para");
//			ST7735_SetCursor(1, 8);
//			ST7735_OutString((char*) "nuevo juego");
//		}
		
		while ((GPIO_PORTE_DATA_R & 0x01) != 1) {}
		

	}
	

}




