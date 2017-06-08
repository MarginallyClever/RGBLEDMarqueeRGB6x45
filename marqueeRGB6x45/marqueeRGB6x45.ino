//------------------------------------------------------------------------------
// LED scrolling message hat
// dan@marginallyclever.com 2017-06-06
// message scrolling around a baseball hat.
// font from http://uzebox.org/wiki/index.php?title=Charset_6x6_192x18.png
// converted with the following processing sketch:
/*
PImage img;

void setup() {
  size(192,18);
  img=loadImage("Charset_6x6_192x18.png");

  for(int y=0;y<img.height;++y) {
    for(int x=0;x<img.width;++x) {
      long c = img.get(x,y);
      print((c==-1?0:1)+",");
    }
    println();
  }
}

void draw() {
  background(0);
  image(img,0,0);
}
*/
//------------------------------------------------------------------------------
#include <Adafruit_NeoPixel.h>

#define VERBOSE 0

#define HELMET_W     45
#define HELMET_H     6
#define NUMPIXELS (HELMET_W*HELMET_H)

#define PIN  12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

// font features
#define LETTER_W (6)
#define LETTER_H (6)
#define FONT_W_PIXELS   (192)
#define FONT_H_PIXELS   (18)
#define FONT_W_CHARS    (32) //FONT_W_PIXELS/LETTER_W
#define FONT_H_CHARS    (3)  //FONT_H_PIXELS/LETTER_H

const char fontMap[] = " !\"#$%&'()*+.-,/0123456789:;<=>?"\
                       "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"\
                       "`abcdefghijklmnopqrstuvwxyz{|}~ ";
const unsigned char PROGMEM font[] = {
0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,
0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,
0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,1,1,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,1,1,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,0,0,1,0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,1,1,1,1,0,1,0,0,1,1,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

0,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,0,0,1,1,1,1,0,1,0,0,0,1,0,0,1,1,1,0,0,0,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
1,0,1,1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,1,0,1,1,0,1,1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,
1,0,1,1,1,0,1,0,0,0,1,0,1,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,0,0,1,1,1,1,0,0,1,0,1,1,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,1,1,1,1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,1,0,1,0,0,0,1,0,1,1,1,1,0,0,1,0,0,1,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,0,1,0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,1,0,0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0,1,0,1,0,1,1,1,1,1,0,
0,0,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,1,0,1,1,0,0,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1,0,0,1,0,0,1,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0,
0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,1,1,0,0,0,0,1,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,0,1,1,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,1,0,
0,0,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,
0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,1,0,1,0,0,1,0,0,1,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};


#define FPS              (30.0)  // animation smoothness
#define SCROLL_STEP_SIZE (0.8f)  // animation speed
#define BACKWARDS        (1)  // set to 0 if you wired your grid backwards
#define ANTIALIAS        (1)  // set to 0 if you don't want blending
#define SWIZZLE(x)       (x)  // white text on black background
//#define SWIZZLE(x)       (1.0-x)  // black text on white background

#define FPS_DELAY       (1000.0/FPS)

#define NUM_MESSAGES 3
//const char *messages[] = {"FIVE ALARM FUNK","NEVER QUIT ON YOUR DREAMS","BEST FRIENDS FOREVER"};
const char *messages[] = {"WAR IS PEACE","FREEDOM IS SLAVERY","IGNORANCE IS STRENGTH"};
const char *currentMessage;
int currentMessageIndex;
int current_message_length;
int message_width_in_pixels;


char fastFontMap[256];


void setup() {
  Serial.begin(57600);
  Serial.println("**START**");
  
  prepareFastMap();
  strip.begin();
}


void loop() {
  displayMessage();
  pacmanScroll();
  displayMessage();
  sineWave();
  displayMessage();
  twinklingStars();
  displayMessage();
  policeLights();
}

const unsigned char PROGMEM  pacmanBitmap[]=
"00rrr000ppp000ccc000ooo0000000yy0000000000000"\
"0rwrwr0pwpwp0cwcwc0owowo00000yyyy000000000000"\
"0rrrrr0ppppp0ccccc0ooooo000000yyyy00000000000"\
"0rrrrr0ppppp0ccccc0ooooo0000000yyy00000000000"\
"0rrrrr0ppppp0ccccc0ooooo00000yyyy000000000000"\
"0r0r0r0p0p0p0c0c0c0o0o0o000000yy0000000000000";


long pacmanColorLookup(char c) {
  switch(c) {
    case 'r': return 0x00FF00;
    case 'p': return 0x00CC44;
    case 'c': return 0x330033;
    case 'w': return 0xFFFFFF;
    case 'o': return 0x44CC00;
    case 'y': return 0xFFFF00;
    default:return 0;
  }
}


void pacmanScroll() {
  int x,y;
  int width = HELMET_W*2;
  long t,endT = millis()+4000, startT=millis();
  while(millis()<endT) {
    t = width * (millis()-startT)/ ( endT-startT);
    for(y=0;y<HELMET_H;++y) {
      for(x=0;x<HELMET_W;++x) {
        int nx = x+t-HELMET_W;
        int ny = HELMET_H-1-y;
        int a = y*HELMET_W+x;
        if(nx>=HELMET_W) nx=HELMET_W-1;
        if(nx<0) nx=0;
        int a2 = ny*HELMET_W+nx;
        
        strip.setPixelColor(a,pacmanColorLookup(pgm_read_byte_near(pacmanBitmap+a2)));
      }
    }
    strip.show();
  }
}


void sineWave() {
  int x,y;
  long t,endT = millis()+4000;
  while(millis()<endT) {
    t = millis()*0.0175f;
    for(x=0;x<HELMET_W;++x) {
      float v=sin(x*0.3f+t)*3+3;
      for(y=0;y<HELMET_H;++y) {
        strip.setPixelColor(y*HELMET_W+x,y<=v?0xff0000:0x0000ff);
      }
    }
    strip.show();
  }
}

void testFirstRow() {
  for(int i=0;i<HELMET_W;++i) {
    strip.setPixelColor(i,strip.Color(255,255,255));
  }
  strip.show();
}


void prepare_new_message() {
  currentMessageIndex = (currentMessageIndex+1)%NUM_MESSAGES;
  currentMessage = messages[currentMessageIndex];
  Serial.print("Preparing '");
  Serial.print(currentMessage);
  Serial.println("'");
  
  current_message_length = strlen(currentMessage);
  Serial.print("length=");  Serial.println(current_message_length);

  message_width_in_pixels = LETTER_W * current_message_length;
  Serial.print("pixels=");  Serial.println(message_width_in_pixels);
}


void displayMessage() {
  strip.clear();
  
  prepare_new_message();
  
  // for animations
  float pixel, antialias;
  float scroll_step=-HELMET_W;  // we start with the message off the right side of the grid
  int x,y;
  long tock;

  do {
    tock=millis();

    if(scroll_step<0) {
      pixel = scroll_step;
      while(pixel<0) pixel++;
      antialias = pixel - (int)pixel;
    } else {
      antialias = scroll_step - (int)scroll_step;
    }

    // for every dot on the LED grid
    for(y=0;y<HELMET_H;++y) {
      for(x=0;x<HELMET_W;++x) {
        float newX = x+scroll_step;
        // find the 1 or 0 in the message to display at this dot
#if ANTIALIAS == 0
        pixel = get_pixel(newX,y);
#else
        pixel = get_pixel(newX  ,y) * (1.0f - antialias)
              + get_pixel(newX+1,y) * (       antialias);
#endif
        pixel = SWIZZLE(pixel);
        
        // draw the pixel
        p( x, y, c(newX,y,pixel) );
        
#if VERBOSE > 2
        Serial.print((char)letter_being_drawn);
        Serial.print(',');
        Serial.print(fy);
        Serial.print(',');
        Serial.print(fx);
        Serial.print(',');
        Serial.print( pixel,DEC);
        Serial.print('\t');
#endif
      }
#if VERBOSE > 2
      Serial.print("\n");
#endif
    }  
    strip.show();
#if VERBOSE > 1
    Serial.print(n);
    Serial.print('-');
    Serial.println(scroll_step);
#endif

    Serial.print(millis() - tock);
    Serial.print("\t");
    Serial.println(FPS_DELAY);
   
    // animation time - adjust scroll_step, which will push the message to the left. 
    while( millis() - tock < FPS_DELAY);
    scroll_step+= SCROLL_STEP_SIZE;
  // stop after the message has gone off the grid.
  } while(scroll_step < message_width_in_pixels + HELMET_W);
}


// Turn (x,y) into the correct pixel address.
// This only works because the lights are wired in an S pattern.
void p(int x,int y,long color) {
  int newY = HELMET_H - 1 - y;
  int a = newY*HELMET_W + x;
  strip.setPixelColor(a,color);
}

#define TEXT_COLORS 8
long textColorTable[TEXT_COLORS] = {
  0xFFFFFF,
  0x9400D3,
  0xFF7F00,
  0x0000FF,
  0x00FF00,
  
  0xFFFF00,
  0x4B0082,
  0xFF0000,
};


// some wavy rainbow color fun
long c(int x,int y,float v) {
  int newX = ((x+1)/LETTER_W);
  long t = textColorTable[newX%TEXT_COLORS];
  long r= (long)(((t>>16) & 0xff)*v)<<16;
  long g= (long)(((t>> 8) & 0xff)*v)<<8;
  long b= (long)(((t    ) & 0xff)*v);
  return r+g+b;
  /*
  return strip.Color(v*(cos(        newX*0.3)*127.0f+127),
                     v*(sin(        newX*0.1)*127.0f+127),
                     v*(sin(3.14159+newX*0.1)*127.0f+127));
  */
}


// translate the pixel on screen (x,y) to a pixel in the bitmap
float get_pixel(int x,int y) {
  int nx,nx2,fx,fy,letter_being_drawn;
  
  nx = x;
  nx2 = 0;
  if( nx < 0 ) {
    letter_being_drawn = 0;
  } else {
    nx = x / LETTER_W;
    if( nx > current_message_length ) {
      letter_being_drawn = 0;
    } else {
      nx2 = x % LETTER_W;
      letter_being_drawn = currentMessage[nx];
    }
  }

  //int fontMapIndex = getFontMapIndexSlow(letter_being_drawn);
  int fontMapIndex = fastFontMap[letter_being_drawn];
    
  fy = ( fontMapIndex / FONT_W_CHARS ) * LETTER_H + y;
  fx = ( fontMapIndex % FONT_W_CHARS ) * LETTER_W + nx2;
  if( fy > FONT_H_PIXELS-1 ) fy = FONT_H_PIXELS-1;
  if( fx > FONT_W_PIXELS-1 ) fx = FONT_W_PIXELS-1;
  
  // if pixel == WHITE_ON_BLACK, turn on the light
  // pgm_read_byte_near() pulls the 1 or 0 from font into RAM
  return pgm_read_byte_near(font + fy * FONT_W_PIXELS + fx);
}


// Find the index into the map for ASCII code c
int getFontMapIndexSlow(int c) {
  int i;
  for(i=0;i<FONT_W_CHARS*FONT_H_CHARS;++i) {
    if(fontMap[i]==c) {
        return i;
    }
  }
  return 0;  // blank space
}


void prepareFastMap() {
  int i;
  for(i=0;i<256;++i) {
    fastFontMap[i]=getFontMapIndexSlow(i);
  }
}


void policeLights() {
  long endTime = millis() + 3000;
  long color;
  long t;
  int a,b,x,y;
  do {
    t = millis()*0.025f;
    for(y=0;y<HELMET_H;++y) {
      for(x=0;x<HELMET_W;++x) {
        a = y*HELMET_W+HELMET_W-x;
        b = (x+t)%10;
        if(b<5) color = strip.Color(0,255,0);
        else    color = strip.Color(0,0,255);
        strip.setPixelColor(a,color);
      }
    }
    strip.show();
  
  } while(millis() < endTime);
}


#define NUMSTARS 30
#define STAR_AGE_MAX 250.0

struct {
  int a;  // address
  float age;  // ms
} typedef Star;
  Star stars[NUMSTARS];



void twinklingStars() {
  strip.clear();
  
  int x,y;
  long t,i;
  for(t=0;t<NUMSTARS;++t) { 
    stars[t].a   = random(0,HELMET_W*HELMET_H);
    stars[t].age = random(0,STAR_AGE_MAX);
  }
  
  long endTime = millis() + 3000;
  long oldT = millis();
  do {
    t = millis();
    long dt = t - oldT;
    oldT=t;
    for(i=0;i<NUMSTARS;++i) {
      stars[i].age+=dt;
      if(stars[i].age>=STAR_AGE_MAX) {
        strip.setPixelColor(stars[i].a,0);
        stars[i].age = 0;
        stars[i].a = random(0,HELMET_W*HELMET_H);
      }
      float ageCurve = sin((stars[i].age/STAR_AGE_MAX)*PI);
      long color = strip.Color(ageCurve*0xFF,ageCurve*0xFF,ageCurve*ageCurve*0x44);
      strip.setPixelColor(stars[i].a,color);
    }
    strip.show();
  } while(millis() < endTime);
}

