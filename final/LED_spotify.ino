#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <FastLED.h>
#include<iostream>

#include<cstdlib>

using namespace std;

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example is an adapted version of Mark Kriegsman's code  
// the BPM function is specifically adapated to account for song information from
// spotify


#define DATA_PIN    2
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    241
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         80
#define FRAMES_PER_SECOND  200

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char messageFromPC[numChars] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;

TFT_eSPI tft = TFT_eSPI();

boolean newData = false;

//============

void setup() {
    Serial.begin(921600);
    Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
    Serial.println("Enter data in this style <HelloWorld, 12, 24.7>  ");
    Serial.println();

    //for testing purposes
    tft.init();
    tft.setRotation(1);

    /* for LED LIGHTS, from the DemoReel100 sample code, modified */
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { bpm, juggle, bpm, confetti, bpm, sinelon};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue; // rotating "base color" used by many of the patterns
//============

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
    /*led lights here?*/
    gPatterns[gCurrentPatternNumber]();

    // send the 'leds' array out to the actual LED strip
    FastLED.show();  
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND); 

    // do some periodic updates
    typedef void (*SimplePatternList[])();
    SimplePatternList gPatterns = {bpm}; 
    EVERY_N_MILLISECONDS( 10 ) { gHue++; } // slowly cycle the "base color" through the rainbow
    //randomly changing the pattern every 7-10 seconds
    int switch_pattern = 7 + (rand() % 8);
    EVERY_N_SECONDS( switch_pattern ) { nextPattern(); } // change patterns periodically
      
    
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

//============ code for handling array input < string, int, float>

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\r'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}
//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    integerFromPC = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    floatFromPC = atof(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
  //this is working, but the data isn't being displayed for some reason...
    Serial.print("Message ");
    Serial.println(messageFromPC);
    Serial.print("Integer ");
    Serial.println(integerFromPC);
    Serial.print("Float ");
    Serial.println(floatFromPC);

  //debugging using the LED esp32 screen
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0, 2);
    tft.setTextColor(TFT_RED,TFT_BLACK);    tft.setTextFont(4);
    tft.println("now playing...");
    tft.println(messageFromPC);
    tft.println(integerFromPC);
    tft.println(floatFromPC);

}
//============ end of input handling

void bpm()
{
  //creating a random BPM range so that the beat isn't just a flat #
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = integerFromPC;
  int random;
  int beat_oscillation;
  gHue = floatFromPC;
  tft.println(gHue);

  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, integerFromPC);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    int min = integerFromPC-40;
    int max = integerFromPC-min+1;
    beat_oscillation = min + (rand() % min); 
       
    //updating beat 
    beat = beatsin8( beat_oscillation, 100, integerFromPC);
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  addGlitter(40);

  
}

void juggle() {
  // twenty colored dots, weaving in and out of sync with each other
  // fadeToBlackBy( leds, NUM_LEDS, 20);
  // uint8_t dothue = 100;
  // for( int i = 0; i < 20; i++) {
  //   leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
  //   dothue += 32;
  // }

  fadeToBlackBy( leds, NUM_LEDS, 5);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  addGlitter(90);
}


void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  // fadeToBlackBy( leds, NUM_LEDS, 40);
  // int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  // leds[pos] += CHSV( gHue, 255, 192);

  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}
void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}
