//tft-related
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <Fonts/OpenSansBold14pt7b.h>
#include <Fonts/OpenSansBold30pt7b.h>
// These are 'flexible' lines that can be changed
#define TFT_CS 17
#define TFT_DC 15
#define TFT_RST -1 // RST can be set to -1 if you tie it to Arduino's reset

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

int TXT_COLOR =HX8357_RED;
int BG_COLOR =HX8357_BLACK;

int pwm_lvl=2000;
