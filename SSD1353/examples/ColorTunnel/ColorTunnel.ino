#include <Arduino.h>

#include "SSD1353.h"

SSD1353 lcd; // Create object 'lcd' of type 'SSD1353'

// Connect the pins of the driver to the following pins
#define CS   2
#define DC   3
#define RS   4
#define DISF 5
#define D0   6
#define D1   7
#define D2   8
#define D3   9
#define D4   10
#define D5   11
#define D6   12
#define D7   13
// Connect Vdd to +3V3 and Vss to GND
// Connect RW (aka WR) to GND
// Leave E (aka RD) floating, or tie to +3V3

void setup() {
  // Initialize the display
  lcd.init(CS, DC, RS, DISF, D0, D1, D2, D3, D4, D5, D6, D7);

  // Initialize random number gen from a random point
  randomSeed(analogRead(A0));
}

void loop() {
  // Pick the random color values
  byte randomR = random(64);
  byte randomG = random(64);
  byte randomB = random(64);

  for (byte i = 63; i > 0; i--) {
    // Draw a rectangle without fill with the random color values
    lcd.drawRectangle(i, i, 159 - i, 127 - i, randomR, randomG, randomB, 0, 0, 0, false);
    delay(5);
  }
}