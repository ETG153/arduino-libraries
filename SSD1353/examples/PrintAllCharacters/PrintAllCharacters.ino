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
  // Print first row of characters @ (0,120) with the color 0x3F, 0, 0 (red)
  lcd.printstr("ABCDEFGHIJKLM", 0, 120, 0x3F, 0, 0);

  // Print the second row of characters in orange
  lcd.printstr("NOPQRSTUVWXYZ", 0, 112, 0x3F, 0x10, 0);

  // Print the third row of characters in yellow
  lcd.printstr("abcdefghijklm", 0, 104, 0x3F, 0x3F, 0);

  // Print the fourth row of characters in green
  lcd.printstr("nopqrstuvwxyz", 0, 96, 0, 0x3F, 0);

  // Print the fifth row of characters in light blue
  lcd.printstr("0123456789&'(", 0, 88, 0, 0x10, 0x3F);

  // Print the sixth row of characters in dark blue
  lcd.printstr(")*+-=.!\"#$%^,", 0, 80, 0, 0, 0x3F);

  // Print the seventh row of characters in purple
  lcd.printstr(":;?@/<>|\\[]{}", 0, 72, 0x0F, 0, 0x3F);

  // Print the eighth row of characters in pink
  lcd.printstr("_~", 0, 64, 0x3F, 0, 0x3F);
}

void loop() {
  // Set the display to normal color mode (default)
  lcd.displayMode(SSD1353_ON);
  delay(1000);

  // Invert the displayed colors
  lcd.displayMode(SSD1353_INVERSE);
  delay(1000);
}