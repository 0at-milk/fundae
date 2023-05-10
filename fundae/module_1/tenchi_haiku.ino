// New background colour
#define TFT_BROWN 0x38E0
#define TFT_BLUE 0x38E0

// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 1500

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

unsigned long targetTime = 0; // Used for testing draw times
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);


void setup() {
  // put your setup code here, to run once:
  tft.init();
  tft.setRotation(1);
  lcd.begin(100,20);

}

void loop() {
  // put your main code here, -to run repeatedly:
  targetTime = millis();

  // First we test them with a background colour set
  tft.setTextSize(1);
  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_BLUE, TFT_BROWN);


  tft.drawString("course the rush mat roof", 0, 0, 2);
  delay(WAIT);
  tft.drawString("sheltering", 0, 16, 4);
  delay(WAIT);
  tft.fillScreen(TFT_BROWN);
  tft.drawString("the harvest hut", 0, 16, 4);
  delay(WAIT);
  tft.drawString("of the autumn", 0, 32, 4);
  delay(WAIT);
  tft.fillScreen(TFT_BLACK);
  tft.drawString("rice fields", 0, 16, 4);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);

  tft.drawString("and my sleeves", 0, 0, 2);
  delay(WAIT);
  tft.drawString("are growing wet", 0, 16, 4);
  delay(WAIT);
  tft.fillScreen(TFT_BLACK);
  tft.drawString("with the moisture", 0, 50, 4);
  delay(WAIT);
  tft.fillScreen(TFT_BLACK);
  tft.drawString("dripping through", 0, 16, 4);
  delay(WAIT);
  // tft.drawString("DONE", 0, 104, 4);
  // delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.drawString("-Tenchi Tenno", 0, 32, 16);
  // tft.drawNumber(millis() - targetTime, 0, 100, 4);
  delay(4000);

}
