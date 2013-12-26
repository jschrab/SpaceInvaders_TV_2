// ------------------------------------------------------------------
// Eternally Self-Destroying Space Invaders
//
// Jeffrey Schrab - jschrab@malicstower.org
// ------------------------------------------------------------------
// Incept Date - 5 JAN 2008
// ------------------------------------------------------------------
// MAJOR contributions drawn from Alastair Parker's ArduinoPong 2007
// ------------------------------------------------------------------

#include <avr/pgmspace.h>

// ------------------------------------------------------------------

//-- Video out voltage levels
#define _SYNC 0x00
#define _BLACK 0x01
#define _GRAY 0x02
#define _WHITE 0x03

//-- Dimensions of the screen
#define WIDTH 38
#define HEIGHT 14

//-- Number of lines to display
#define DISPLAY_LINES 240

//-- Video pins
#define DATA_PIN 9
#define SYNC_PIN 8

//-- Artwork bitmaps 
#define ___ 0x01
#define _X_ 0x03

// ------------------------------------------------------------------

//-- Store our artwork in program flash memory instead of precious RAM
const prog_uint8_t PROGMEM siFrameList[] = { 0, 1, 0, 1, 0, 1, 0, 1, 6, 2, 3, 2, 3, 2, 3, 2, 3, 6, 4, 5, 4, 5, 4, 5, 4, 5, 6 };
const prog_uint8_t PROGMEM siBitmap[][9][12] = {
    //-- Invader 1 - 2 Frames
    {
      //          0    1    2    3    4    5    6    7    8    9    A    B
      /* 0 */  { ___, ___, ___, _X_, ___, ___, ___, ___, ___, _X_, ___, ___ },
      /* 1 */  { ___, _X_, ___, ___, _X_, ___, ___, ___, _X_, ___, ___, _X_ },
      /* 2 */  { ___, _X_, ___, _X_, _X_, _X_, _X_, _X_, _X_, _X_, ___, _X_ },
      /* 3 */  { ___, _X_, ___, _X_, ___, _X_, _X_, _X_, ___, _X_, ___, _X_ },
      /* 4 */  { ___, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_ },
      /* 5 */  { ___, ___, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, ___ },
      /* 6 */  { ___, ___, ___, _X_, ___, ___, ___, ___, ___, _X_, ___, ___ },
      /* 7 */  { ___, ___, _X_, ___, ___, ___, ___, ___, ___, ___, _X_, ___ },
      /* 8 */  { ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___ }
    },
    {
      //          0    1    2    3    4    5    6    7    8    9    A    B
      /* 0 */  { ___, ___, ___, _X_, ___, ___, ___, ___, ___, _X_, ___, ___ },
      /* 1 */  { ___, ___, ___, ___, _X_, ___, ___, ___, _X_, ___, ___, ___ },
      /* 2 */  { ___, ___, ___, _X_, _X_, _X_, _X_, _X_, _X_, _X_, ___, ___ },
      /* 3 */  { ___, ___, ___, _X_, ___, _X_, _X_, _X_, ___, _X_, ___, ___ },
      /* 4 */  { ___, ___, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, ___ },
      /* 5 */  { ___, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_ },
      /* 6 */  { ___, _X_, ___, _X_, ___, ___, ___, ___, ___, _X_, ___, _X_ },
      /* 7 */  { ___, _X_, ___, ___, _X_, _X_, ___, _X_, _X_, ___, ___, _X_ },
      /* 8 */  { ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___ }
    },
    //-- Invader 2 - 2 Frames
    {
      //          0    1    2    3    4    5    6    7    8    9    A    B
      /* 0 */  { ___, ___, ___, ___, ___, _X_, _X_, ___, ___, ___, ___, ___ },
      /* 1 */  { ___, ___, ___, ___, _X_, _X_, _X_, _X_, ___, ___, ___, ___ },
      /* 2 */  { ___, ___, ___, _X_, _X_, _X_, _X_, _X_, _X_, ___, ___, ___ },
      /* 3 */  { ___, ___, _X_, _X_, ___, _X_, _X_, ___, _X_, _X_, ___, ___ },
      /* 4 */  { ___, ___, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, ___, ___ },
      /* 5 */  { ___, ___, ___, ___, _X_, ___, ___, _X_, ___, ___, ___, ___ },
      /* 6 */  { ___, ___, ___, _X_, ___, _X_, _X_, ___, _X_, ___, ___, ___ },
      /* 7 */  { ___, ___, _X_, ___, _X_, ___, ___, _X_, ___, _X_, ___, ___ },
      /* 8 */  { ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___ }
    },
    {
      //          0    1    2    3    4    5    6    7    8    9    A    B
      /* 0 */  { ___, ___, ___, ___, ___, _X_, _X_, ___, ___, ___, ___, ___ },
      /* 1 */  { ___, ___, ___, ___, _X_, _X_, _X_, _X_, ___, ___, ___, ___ },
      /* 2 */  { ___, ___, ___, _X_, _X_, _X_, _X_, _X_, _X_, ___, ___, ___ },
      /* 3 */  { ___, ___, _X_, _X_, ___, _X_, _X_, ___, _X_, _X_, ___, ___ },
      /* 4 */  { ___, ___, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, ___, ___ },
      /* 5 */  { ___, ___, ___, _X_, ___, _X_, _X_, ___, _X_, ___, ___, ___ },
      /* 6 */  { ___, ___, _X_, ___, ___, ___, ___, ___, ___, _X_, ___, ___ },
      /* 7 */  { ___, ___, ___, _X_, ___, ___, ___, ___, _X_, ___, ___, ___ },
      /* 8 */  { ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___ }
    },
    //-- Invader 3 - 2 Frames
    {
      //          0    1    2    3    4    5    6    7    8    9    A    B
      /* 0 */  { ___, ___, ___, ___, _X_, _X_, _X_, _X_, ___, ___, ___, ___ },
      /* 1 */  { ___, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, ___ },
      /* 2 */  { _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_ },
      /* 3 */  { _X_, _X_, _X_, ___, ___, _X_, _X_, ___, ___, _X_, _X_, _X_ },
      /* 4 */  { _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_ },
      /* 5 */  { ___, ___, ___, _X_, _X_, ___, ___, _X_, _X_, ___, ___, ___ },
      /* 6 */  { ___, ___, _X_, _X_, ___, ___, ___, ___, _X_, _X_, ___, ___ },
      /* 7 */  { _X_, _X_, ___, ___, ___, _X_, _X_, ___, ___, ___, _X_, _X_ },
      /* 8 */  { ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___ }
   },
   {
      //          0    1    2    3    4    5    6    7    8    9    A    B
      /* 0 */  { ___, ___, ___, ___, _X_, _X_, _X_, _X_, ___, ___, ___, ___ },
      /* 1 */  { ___, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, ___ },
      /* 2 */  { _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_ },
      /* 3 */  { _X_, _X_, _X_, ___, ___, _X_, _X_, ___, ___, _X_, _X_, _X_ },
      /* 4 */  { _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_ },
      /* 5 */  { ___, ___, _X_, _X_, _X_, ___, ___, _X_, _X_, _X_, ___, ___ },
      /* 6 */  { ___, _X_, _X_, ___, ___, _X_, _X_, ___, ___, _X_, _X_, ___ },
      /* 7 */  { ___, ___, _X_, _X_, ___, ___, ___, ___, _X_, _X_, ___, ___ },
      /* 8 */  { ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___ }
    },
    //-- Invader Destroyed - 1 Frame
    {
      //          0    1    2    3    4    5    6    7    8    9    A    B
      /* 0 */  { ___, ___, ___, ___, _X_, ___, ___, ___, _X_, ___, ___, ___ },
      /* 1 */  { ___, ___, _X_, ___, ___, _X_, ___, _X_, ___, ___, _X_, ___ },
      /* 2 */  { ___, ___, ___, _X_, ___, ___, ___, ___, ___, _X_, ___, ___ },
      /* 3 */  { ___, ___, ___, ___, _X_, ___, ___, ___, _X_, ___, ___, ___ },
      /* 4 */  { ___, _X_, _X_, ___, ___, ___, ___, ___, ___, ___, _X_, _X_ },
      /* 5 */  { ___, ___, ___, ___, _X_, ___, ___, ___, _X_, ___, ___, ___ },
      /* 6 */  { ___, ___, ___, _X_, ___, ___, ___, ___, ___, _X_, ___, ___ },
      /* 7 */  { ___, ___, _X_, ___, ___, _X_, ___, _X_, ___, ___, _X_, ___ },
      /* 8 */  { ___, ___, ___, ___, _X_, ___, ___, ___, _X_, ___, ___, ___ }
   }
};

// ------------------------------------------------------------------

byte newLine = 0;
int pauseCount = 0;

// ------------------------------------------------------------------

//-- the video frameBuffer
byte frameBuffer[WIDTH][HEIGHT];

// ------------------------------------------------------------------

void setPixel(byte x, byte y, byte color) {
	frameBuffer[x][y]= color;
} // setPixel

// ------------------------------------------------------------------

void setFrameBufferTo(byte frame) {
	for(int x=0; x<12; x++) {
		for(int y=0; y<8; y++) {
			//-- Retrieve our artwork bytes from program flash memory instead of precious RAM
			if(pgm_read_byte(&siBitmap[frame][y][x])==_X_) {
				setPixel((x*2)+6,y+3,_WHITE);
				setPixel((x*2)+6+1,y+3,_WHITE);
			} else {
				setPixel((x*2)+6,y+3,_BLACK);
				setPixel((x*2)+6+1,y+3,_BLACK);
			} // if
		} // for..y
	} // for..x
} // setFrameBufferTo

// ------------------------------------------------------------------

void clearScreen() {
	for (int x = 0; x < WIDTH; x++) {
		for (int y=0; y <= HEIGHT; ++y) {
			frameBuffer[x][y] = _BLACK;
		} // for..y
    } // for..x
} // clearScreen

// ------------------------------------------------------------------

void setup() {
	cli();
	pinMode(SYNC_PIN, OUTPUT);
	pinMode(DATA_PIN, OUTPUT);
	digitalWrite(SYNC_PIN, HIGH);
	digitalWrite(DATA_PIN, HIGH);
	clearScreen();
	setFrameBufferTo(0);
} // setup

// ------------------------------------------------------------------

void loop() {
	//-- iterate over the lines on the tv
	for(int line = 0; line < DISPLAY_LINES; ++line) {
		// HSync
		// front porch (1.5 us)
		PORTB = _BLACK; delayMicroseconds(1.5);

		// sync (4.7 us)
		PORTB = _SYNC; delayMicroseconds(4.7);

		// breezeway (.6us) + burst (2.5us) + colour back borch (1.6 us)
		PORTB = _BLACK; delayMicroseconds(0.6+2.5+1.6);

		//calculate which line to draw to
		newLine = line>>4; delayMicroseconds(1);

		// display the array for this line
		// a loop would have been smaller, but it messes the timing up
		PORTB = frameBuffer[0][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[1][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[2][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[3][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[4][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[5][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[6][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[7][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[8][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[9][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[10][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[11][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[12][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[13][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[14][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[15][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[16][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[17][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[18][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[19][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[20][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[21][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[22][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[23][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[24][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[25][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[26][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[27][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[28][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[29][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[30][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[31][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[32][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[33][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[34][newLine]; delayMicroseconds(1);
		PORTB = frameBuffer[35][newLine]; delayMicroseconds(1);

		// kludge to correct timings
		PORTB = frameBuffer[36][newLine];
		PORTB=PORTB;
		PORTB=PORTB;
		PORTB=PORTB;
		delayMicroseconds(2);
	} // for

	// vsync
	PORTB = _SYNC;

	setFrameBufferTo(pgm_read_byte(&siFrameList[++pauseCount / 32 % sizeof(siFrameList)]));

	// wait for the remainder of the sync period
	// delayMicroseconds(565);
} // loop

// ------------------------------------------------------------------

