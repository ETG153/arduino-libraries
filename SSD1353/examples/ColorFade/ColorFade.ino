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
}

void loop() {
  // Fade display from red to green
  for (byte g = 0; g < 64; g++) {
    lcd.fill(63 - g, g, 0);
    delay(100);
  }
  delay(500);

  // Fade display from green to blue
  for (byte b = 0; b < 64; b++) {
    lcd.fill(0, 63 - b, b);
    delay(100);
  }
  delay(500);

  // Fade display from blue to red
  for (byte r = 0; r < 64; r++) {
    lcd.fill(r, 0, 63 - r);
    delay(100);
  }
  delay(500);
}