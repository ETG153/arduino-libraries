/*
  SSD1353.h -
    Copyright (C) 2022 Paulus Kivelä
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SSD1353_H
#define SSD1353_H

#include <stdint.h>

#define SSD1353_OFF     0xA6
#define SSD1353_ON      0xA4
#define SSD1353_INVERSE 0xA7

class SSD1353 {
  public:
  void init(uint8_t pinCS, uint8_t pinDC, uint8_t pinRS, uint8_t pinDISF, uint8_t pinD0, uint8_t pinD1, uint8_t pinD2, uint8_t pinD3, uint8_t pinD4, uint8_t pinD5, uint8_t pinD6, uint8_t pinD7);
  void displayMode(uint8_t mode);
  void drawRectangle(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t lineRed, uint8_t lineGreen, uint8_t lineBlue, uint8_t fillRed, uint8_t fillGreen, uint8_t fillBlue, bool useFill);
  void drawLine(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t lineRed, uint8_t lineGreen, uint8_t lineBlue);
  void drawDot(uint8_t x, uint8_t y, uint8_t valueRed, uint8_t valueGreen, uint8_t valueBlue);
  void fill(uint8_t valueRed, uint8_t valueGreen, uint8_t valueBlue);
  void clear();
  void printchar(uint8_t x, uint8_t y, char character, uint8_t valueRed, uint8_t valueGreen, uint8_t valueBlue);
  void printstr(const char* input, uint8_t x, uint8_t y, uint8_t valueRed, uint8_t valueGreen, uint8_t valueBlue);

  private:
  uint8_t _pinCS,
      _pinDC,
      _pinRS,
      _pinDISF;
  uint8_t _pinD[8];
  
  void write(uint8_t data, bool isCommand);
  void enableFill(bool enable);
};

#endif
