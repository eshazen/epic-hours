//
// Control firmware for the EPIC LED sign
//   8 display chains, first 6 with (6) 8x8 segments, last one with (8) 8x8 segments
// Expect serial commands at 9600 baud on hardware UART:
//
// M <marquee text>          update the top-line scrolled message (80 char max)
// O n hh:mm                 update opening time for day n (0-7)
// C n hh:mm                 update closing time for day n (0-7)
// I n                       set intensity to n (0-10)
//
// Note that times must be 12-hour (there isn't room for a digit "2" in the 1st position)
//

#include <LEDMatrixDriver.hpp>

const uint8_t cs_pins[] = { 6, A0, A1, A2, A3, A4, A5, 7 };
#define NDISP (sizeof(cs_pins)/sizeof(cs_pins[0]))
#define TOPDISP (NDISP-1)

// Number of 8x8 segments you are connecting
const int LEDMATRIX_SEGMENTS = 6; // weekday displays
const int TOPMATRIX_SEGMENTS = 8; // top display

const int TOPMATRIX_WIDTH    = TOPMATRIX_SEGMENTS * 8;
const int LEDMATRIX_WIDTH    = LEDMATRIX_SEGMENTS * 8;

// The LEDMatrixDriver class instance
// LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);
LEDMatrixDriver* lmdp[NDISP];

// Buffer to hours display
char t_open[6];
char t_clos[6];
  
// command input buffer
char buff[80];

// token pointers
char *tokn[5];

// scrolling text buffer
char scrolling[80];

// Top line marquee speed (lower nubmers = faster)
const int ANIM_DELAY = 30;

// one framebuffer
uint8_t fb[TOPMATRIX_SEGMENTS*8];

// Time update delay for demo (1 sec)
const int TIME_DELAY = 1000;

unsigned long last_ms;
unsigned long time_ms;

uint8_t dayn = 0;		// day number for update
uint8_t utime = 0;		// update flag for times
uint8_t nscr = 0;		// update flag for scrolling
uint8_t brgt = 0;		// brightness

byte font_char[9];		// buffer for one font character

int ntok;

// Special 5x7 font with digits 0-9 only, space
// variable width, width is first byte
// ** used only for weekday times **
// Font stored in flash
const byte font[12][8] PROGMEM = { { 5, 0x70,0x88,0x88,0x88,0x88,0x88,0x70}, // 0
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

 // This is the font definition. You can use http://gurgleapps.com/tools/matrix to create your own font or sprites.
 // If you like the font feel free to use it. I created it myself and donate it to the public domain.
 const byte bigfont[95][8] PROGMEM = { {0,0,0,0,0,0,0,0},           // SPACE [32]
                      {0x10,0x18,0x18,0x18,0x18,0x00,0x18,0x18}, // EXCL [33]
                      {0x28,0x28,0x08,0x00,0x00,0x00,0x00,0x00}, // QUOT
                      {0x00,0x0a,0x7f,0x14,0x28,0xfe,0x50,0x00}, // #
                      {0x10,0x38,0x54,0x70,0x1c,0x54,0x38,0x10}, // $
                      {0x00,0x60,0x66,0x08,0x10,0x66,0x06,0x00}, // %
                      {0,0,0,0,0,0,0,0}, // &
                      {0x00,0x10,0x18,0x18,0x08,0x00,0x00,0x00}, // '
                      {0x02,0x04,0x08,0x08,0x08,0x08,0x08,0x04}, // (
                      {0x40,0x20,0x10,0x10,0x10,0x10,0x10,0x20}, // )
                      {0x00,0x10,0x54,0x38,0x10,0x38,0x54,0x10}, // *
                      {0x00,0x08,0x08,0x08,0x7f,0x08,0x08,0x08}, // +
                      {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x08}, // COMMA
                      {0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x00}, // -
                      {0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06}, // DOT
                      {0x00,0x04,0x04,0x08,0x10,0x20,0x40,0x40}, // /
                      {0x00,0x38,0x44,0x4c,0x54,0x64,0x44,0x38}, // 0
                      {0x04,0x0c,0x14,0x24,0x04,0x04,0x04,0x04}, // 1
                      {0x00,0x30,0x48,0x04,0x04,0x38,0x40,0x7c}, // 2
                      {0x00,0x38,0x04,0x04,0x18,0x04,0x44,0x38}, // 3
                      {0x00,0x04,0x0c,0x14,0x24,0x7e,0x04,0x04}, // 4
                      {0x00,0x7c,0x40,0x40,0x78,0x04,0x04,0x38}, // 5
                      {0x00,0x38,0x40,0x40,0x78,0x44,0x44,0x38}, // 6
                      {0x00,0x7c,0x04,0x04,0x08,0x08,0x10,0x10}, // 7
                      {0x00,0x3c,0x44,0x44,0x38,0x44,0x44,0x78}, // 8
                      {0x00,0x38,0x44,0x44,0x3c,0x04,0x04,0x78}, // 9
                      {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00}, // :
                      {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x08}, // ;
                      {0x00,0x10,0x20,0x40,0x80,0x40,0x20,0x10}, // <
                      {0x00,0x00,0x7e,0x00,0x00,0xfc,0x00,0x00}, // =
                      {0x00,0x08,0x04,0x02,0x01,0x02,0x04,0x08}, // >
                      {0x00,0x38,0x44,0x04,0x08,0x10,0x00,0x10}, // ?
                      {0x00,0x30,0x48,0xba,0xba,0x84,0x78,0x00}, // @
                      {0x00,0x1c,0x22,0x42,0x42,0x7e,0x42,0x42}, // A
                      {0x00,0x78,0x44,0x44,0x78,0x44,0x44,0x7c}, // B
                      {0x00,0x3c,0x44,0x40,0x40,0x40,0x44,0x7c}, // C
                      {0x00,0x7c,0x42,0x42,0x42,0x42,0x44,0x78}, // D
                      {0x00,0x78,0x40,0x40,0x70,0x40,0x40,0x7c}, // E
                      {0x00,0x7c,0x40,0x40,0x78,0x40,0x40,0x40}, // F
                      {0x00,0x3c,0x40,0x40,0x5c,0x44,0x44,0x78}, // G
                      {0x00,0x42,0x42,0x42,0x7e,0x42,0x42,0x42}, // H
                      {0x00,0x7c,0x10,0x10,0x10,0x10,0x10,0x7e}, // I
                      {0x00,0x7e,0x02,0x02,0x02,0x02,0x04,0x38}, // J
                      {0x00,0x44,0x48,0x50,0x60,0x50,0x48,0x44}, // K
                      {0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x7c}, // L
                      {0x00,0x82,0xc6,0xaa,0x92,0x82,0x82,0x82}, // M
                      {0x00,0x42,0x42,0x62,0x52,0x4a,0x46,0x42}, // N
                      {0x00,0x3c,0x42,0x42,0x42,0x42,0x44,0x38}, // O
                      {0x00,0x78,0x44,0x44,0x48,0x70,0x40,0x40}, // P
                      {0x00,0x3c,0x42,0x42,0x52,0x4a,0x44,0x3a}, // Q
                      {0x00,0x78,0x44,0x44,0x78,0x50,0x48,0x44}, // R
                      {0x00,0x38,0x40,0x40,0x38,0x04,0x04,0x78}, // S
                      {0x00,0x7e,0x90,0x10,0x10,0x10,0x10,0x10}, // T
                      {0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x3e}, // U
                      {0x00,0x42,0x42,0x42,0x42,0x44,0x28,0x10}, // V
                      {0x80,0x82,0x82,0x92,0x92,0x92,0x94,0x78}, // W
                      {0x00,0x42,0x42,0x24,0x18,0x24,0x42,0x42}, // X
                      {0x00,0x44,0x44,0x28,0x10,0x10,0x10,0x10}, // Y
                      {0x00,0x7c,0x04,0x08,0x7c,0x20,0x40,0xfe}, // Z [90]
                       // (the font does not contain any lower case letters. you can add your own.)
                   };    // {}, //



//
// split a comma-separated (or other delim) string to up to siz tokens
// return token count
//
// NOTE:  overwrites string by terminating tokens with '\0'
#
#define SPLIT_DELIM ' '

int split( char *str, char **toks, int siz) {
  char *p = str;		/* point to start of string */
  int nt = 0;			/* token counter */
  
  while( *p && nt < siz) {  /* loop to string end or over siz */
    toks[nt++] = p;	    /* always point to token here */
    while( *p && *p != SPLIT_DELIM)	/* skip over non-delims */
      ++p;
    // check if delim or end of string
    if( *p) {  // non-null char must be delim
      // check for empty token
      if( !p[1])	    /* is this the end of the string */
	toks[nt++] = p+1;   /* yes, add pointer to enpty token */
      *p = '\0';	    /* and terminate it */
      ++p;
    }
  }
  return nt;
}

uint8_t hour = 12;
uint8_t minu = 34;

int xs=0;


void setup() {
  for( uint8_t i=0; i<NDISP; i++) {
    if( i == TOPDISP)
      // flags=7 to invert the display, 0 for normal
      lmdp[i] = new LEDMatrixDriver(TOPMATRIX_SEGMENTS, cs_pins[i], 7, fb);
    else
      lmdp[i] = new LEDMatrixDriver(LEDMATRIX_SEGMENTS, cs_pins[i], 7, fb);
    // init the display
    lmdp[i]->setEnabled(true);
    lmdp[i]->setIntensity(0);   // 0 = low, 10 = high
    lmdp[i]->clear();
    lmdp[i]->display();
  }
  last_ms = millis();
  time_ms = millis();

  Serial.begin(9600);
  Serial.setTimeout(10000);

  strcpy_P( scrolling, PSTR("Welcome to EPIC!"));

  // blank the top display
  lmdp[TOPDISP]->clear();
  lmdp[TOPDISP]->display();

  delay(100);

  strcpy_P( t_open, PSTR("9:00"));
  strcpy_P( t_clos, PSTR("5:00"));
  
  // initial update of all displays
  for( uint8_t i=0; i<NDISP-1; i++) {
    lmdp[i]->clear();
    lmdp[i]->display();
    delay(100);
    *t_open = '0'+i;
    *t_clos = '0'+(7-i);
    drawStringN(lmdp[i], t_open, 5, 0, 0);
    drawStringN(lmdp[i], t_clos, 5, 24, 0);
    lmdp[i]->display();
  }
}

// overwrite first control character with \0
void chomp( char *buff) {
  while( *buff && !iscntrl( *buff))
    ++buff;
  *buff = '\0';
}


void loop()
{
  unsigned long ms_now = millis();

  // check for characters
  if( Serial.available()) {
    // read a complete line
    memset( buff, 0, sizeof(buff));
    Serial.readBytesUntil( '\n', buff, sizeof(buff)-1);
    chomp( buff);
    switch( toupper( buff[0])) {

    case 'M':			// set marquee text
      strcpy( scrolling, buff+2);
      nscr = 1;
      break;

    case 'T':			// update times for a day
      //      int split( char *str, char **toks, int siz) {
      ntok = split( buff, tokn, sizeof(tokn)/sizeof(tokn[0]));
      if( ntok == 4) {
	dayn = *tokn[1]-'0';
	memset( t_open, 0, sizeof(t_open));
	strncpy( t_open, tokn[2], sizeof(t_open));
	memset( t_clos, 0, sizeof(t_clos));
	strncpy( t_clos, tokn[3], sizeof(t_clos));
	utime = 1;
      }
      break;

    case 'I':			// set intensity
      brgt = buff[2]-'0';
      for( int i=0; i<NDISP; i++)
	      lmdp[i]->setIntensity(brgt);   // 0 = low, 10 = high

      break;
    }
  }

  // check for scrolling display update
  
  if( ms_now - last_ms > ANIM_DELAY) {
    last_ms = ms_now;

    if( nscr) {
      nscr = 0;
      lmdp[TOPDISP]->clear();
      lmdp[TOPDISP]->display();
      xs = 0;
    }

    // text message on top, scrolling
    int len = strlen( scrolling);
    drawStringW(lmdp[TOPDISP], scrolling, len, xs, 0);

    // Toggle display of the new framebuffer
    lmdp[TOPDISP]->display();

    // Advance to next coordinate
    if( --xs < len * -8 ) 
      xs = TOPMATRIX_WIDTH;
  }

  // check for others update
  if( utime) {
    utime = 0;

    int i = dayn;
    lmdp[i]->clear();

    // update opening time
    drawStringN(lmdp[i], t_open, 5, 0, 0);

    // update closing time
    drawStringN(lmdp[i], t_clos, 5, 24, 0);

    // Toggle display of the new framebuffer
    lmdp[i]->display();
  }
  
}



/**
 * drawStringN - This function draws a string of the given length to the given position.
 * uses special Narrow font with only digits 0-9, space, :
 */
void drawStringN( LEDMatrixDriver* lmd, char* text, int len, int x, int y )
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

      int w = pgm_read_byte( &(font[c][0]));
      for( uint8_t i=0; i<8; i++)
	font_char[i] = pgm_read_byte( &(font[c][i]));

      // only draw if char is visible
      if( 8 + x + idx * 8 > 0 ) {
	drawSprite( lmd, font_char, x, y, w, 8 );
      }
      x += w+1; 
    }
}

/**
 * drawStringW - This function draws a string of the given length to the given position.
 * uses full ascii uppercase font (characters 32-90)
 */
void drawStringW( LEDMatrixDriver* lmd, char* text, int len, int x, int y )
{
  for( int idx = 0; idx < len; idx ++ )
  {
    int c = toupper(text[idx]) - 32;

    // stop if char is outside visible area
    if( x + idx * 8  > TOPMATRIX_WIDTH )
      return;

    for( uint8_t i=0; i<8; i++)
      font_char[i] = pgm_read_byte( &(bigfont[c][i]));

    // only draw if char is visible
    if( 8 + x + idx * 8 > 0 )
      drawSprite( lmd, font_char, x + idx * 8, y, 8, 8 );
  }
}




/**
 * This draws a sprite to the given position using the width and height supplied (usually 8x8)
 */
void drawSprite( LEDMatrixDriver* lmd, byte* sprite, int x, int y, int width, int height )
{
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;

  for( int iy = 0; iy < height; iy++ )
    {
      for( int ix = 0; ix < width; ix++ )
	{
	  lmd->setPixel(x + ix, y + iy, (bool)(sprite[iy] & mask ));

	  // shift the mask by one pixel to the right
	  mask = mask >> 1;
	}

      // reset column mask
      mask = B10000000;
    }
}

