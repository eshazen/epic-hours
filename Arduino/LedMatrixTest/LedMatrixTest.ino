#include <LEDMatrixDriver.hpp>

// This sketch draw marquee text on your LED matrix using the hardware SPI driver Library by Bartosz Bielawski.
// Example written 16.06.2017 by Marko Oette, www.oette.info

// Define the ChipSelect pin for the led matrix (Dont use the SS or MISO pin of your Arduino!)
// Other pins are Arduino specific SPI pins (MOSI=DIN, SCK=CLK of the LEDMatrix) see https://www.arduino.cc/en/Reference/SPI
const uint8_t LEDMATRIX_CS_PIN = 9;

// Number of 8x8 segments you are connecting
const int LEDMATRIX_SEGMENTS = 6;
const int LEDMATRIX_WIDTH    = LEDMATRIX_SEGMENTS * 8;

// The LEDMatrixDriver class instance
LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);

// Marquee text
char text[] = "12:34 11:00";

// Marquee speed (lower nubmers = faster)
const int ANIM_DELAY = 30;

void setup() {
  // init the display
  lmd.setEnabled(true);
  lmd.setIntensity(4);   // 0 = low, 10 = high
}

int x=0, y=0;   // start top left

// 5x7 font with digits 0-9 only, space
// variable width, width is first byte
byte font[12][8] = { { 5, 0x70,0x88,0x88,0x88,0x88,0x88,0x70}, // 0
                           { 3, 0x40,0xc0,0x40,0x40,0x40,0x40,0xe0}, // 1
                           { 5, 0x70,0x88,0x08,0x10,0x20,0x40,0xf8}, // 2
                           { 5, 0x70,0x88,0x08,0x70,0x08,0x88,0x70}, // 3
                           { 5, 0x10,0x30,0x50,0x90,0xf8,0x10,0x10}, // 4
                           { 5, 0xf8,0x80,0x80,0xf0,0x08,0x08,0xf0}, // 5
                           { 5, 0x70,0x88,0x80,0xf0,0x88,0x88,0x70}, // 6
                           { 5, 0xf8,0x08,0x10,0x20,0x20,0x40,0x40},  // 7
                           { 5, 0x70,0x88,0x88,0x70,0x88,0x88,0x70}, // 8
                           { 5, 0x70,0x88,0x88,0x78,0x08,0x08,0x70}, // 9
                           { 5, 0, 0, 0, 0, 0, 0, 0}, // space
                           { 1, 0x00,0x80,0x80,0x00,0x80,0x80,0x00} // :
};

uint8_t hour = 12;
uint8_t minu = 34;

void loop()
{
	lmd.clear();
  lmd.display();
  // delay(100);
  snprintf( text, sizeof(text), "%02d:%02d", hour, minu);
  if( ++minu > 59) {
    minu = 0;
    if( ++hour > 12)
      hour = 1;
  }

  // Draw the text to the current position
	int len = strlen(text);
	drawString(text, len, x, 0);
	// In case you wonder why we don't have to call lmd.clear() in every loop: The font has a opaque (black) background...

	// Toggle display of the new framebuffer
	lmd.display();

	// Wait to let the human read the display
	delay(1000);


	
}


/**
 * This function draws a string of the given length to the given position.
 */
void drawString(char* text, int len, int x, int y )
{
  for( int idx = 0; idx < len; idx ++ )
  {
    int c = text[idx];
    if( isdigit(c))
      c -= '0';
    else if( c == ':')
      c = 11;
    else
      c = 10;

    int w = font[c][0];

    // only draw if char is visible
    if( 8 + x + idx * 8 > 0 ) {
      drawSprite( font[c]+1, x, y, w, 8 );
    }
    x += w+1; 
  }
}

/**
 * This draws a sprite to the given position using the width and height supplied (usually 8x8)
 */
void drawSprite( byte* sprite, int x, int y, int width, int height )
{
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;

  for( int iy = 0; iy < height; iy++ )
  {
    for( int ix = 0; ix < width; ix++ )
    {
      lmd.setPixel(x + ix, y + iy, (bool)(sprite[iy] & mask ));

      // shift the mask by one pixel to the right
      mask = mask >> 1;
    }

    // reset column mask
    mask = B10000000;
  }
}

