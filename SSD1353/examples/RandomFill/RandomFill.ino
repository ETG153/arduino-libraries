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
  switch (random(2)) {
    case 0:
      // Add 100 randomly placed and colored pixels
      for (size_t i = 0; i < 100; i++) {
        lcd.drawDot(random(160), random(128), random(64), random(64), random(64));
      }
      break;

    case 1:
      // Add a randomly placed and colored line
      lcd.drawLine(random(160), random(128), random(160), random(128), random(64), random(64), random(64));
      break;
  }
}