/*
  SSD1353.cpp -
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

#include "SSD1353.h"
#include <Arduino.h>

/**
 * @brief SSD1353 initialization function
 *
 * @param pinCS Display pin CS
 * @param pinDC Display pin DC
 * @param pinRS Display pin RS
 * @param pinDISF Display pin DISF
 * @param pinD0 Display pin D0
 * @param pinD1 Display pin D1
 * @param pinD2 Display pin D2
 * @param pinD3 Display pin D3
 * @param pinD4 Display pin D4
 * @param pinD5 Display pin D5
 * @param pinD6 Display pin D6
 * @param pinD7 Display pin D7
 */
void SSD1353::init(uint8_t pinCS, uint8_t pinDC, uint8_t pinRS, uint8_t pinDISF, uint8_t pinD0, uint8_t pinD1, uint8_t pinD2, uint8_t pinD3, uint8_t pinD4, uint8_t pinD5, uint8_t pinD6, uint8_t pinD7) {
  _pinCS   = pinCS;
  _pinDC   = pinDC;
  _pinRS   = pinRS;
  _pinDISF = pinDISF;
  _pinD[0] = pinD0;
  _pinD[1] = pinD1;
  _pinD[2] = pinD2;
  _pinD[3] = pinD3;
  _pinD[4] = pinD4;
  _pinD[5] = pinD5;
  _pinD[6] = pinD6;
  _pinD[7] = pinD7;

  pinMode(_pinCS, OUTPUT);
  pinMode(_pinDC, OUTPUT);
  pinMode(_pinRS, OUTPUT);
  pinMode(_pinDISF, OUTPUT);
  for (uint8_t i = 0; i < 8; i++)
    pinMode(_pinD[i], OUTPUT);

  // Display reset sequence
  digitalWrite(_pinDISF, LOW);
  digitalWrite(_pinRS, LOW);
  delay(10);
  digitalWrite(_pinRS, HIGH);
  delay(10);

  // Turn display on
  write(0xAF, true);
  delay(200);
  write(0xA0, true);
  write(0xE0, false);

  // Set contrast
  write(0x81, true);
  write(0xFF, false);
  write(0x82, true);
  write(0xFF, false);
  write(0x83, true);
  write(0xFF, false);

  // Set master current
  write(0x87, true);
  write(0x0F, false);

  // Clear and then enable the display
  clear();
  digitalWrite(_pinDISF, HIGH);
}

/**
 * @brief Write data to driver
 *
 * @param data Byte of data to be written
 * @param isCommand Byte is a command, true/false
 */
void SSD1353::write(uint8_t data, bool isCommand) {
  digitalWrite(_pinDC, !isCommand);
  digitalWrite(_pinCS, LOW);
  for (uint8_t i = 0; i < 8; i++)
    digitalWrite(_pinD[i], ((data >> i) & 1));
  digitalWrite(_pinCS, HIGH);
}

/**
 * @brief Set display operating mode
 *
 * @param mode Acceptable values: SSD1353_OFF, SSD1353_ON, SSD1353_INVERSE
 */
void SSD1353::displayMode(uint8_t mode) {
  if (mode == SSD1353_OFF || mode == SSD1353_ON || mode == SSD1353_INVERSE)
    write(mode, true);
}

/**
 * @brief Enable/disable rectangle fill on drawRectangle
 *
 * @param enable Enable rectangle fill, true/false
 */
void SSD1353::enableFill(bool enable) {
  write(0x26, true);
  write(0 + enable, false);
}

/**
 * @brief Draw a rectangle on the display
 *
 * @param startX Bottom left corner X coordinate
 * @param startY Bottom left corner Y coordinate
 * @param endX Top right corner X coordinate
 * @param endY Top right corner Y coordinate
 * @param lineRed Border line red color value
 * @param lineGreen Border line green color value
 * @param lineBlue Border line blue color value
 * @param fillRed Rectangle fill red color value
 * @param fillGreen Rectangle fill green color value
 * @param fillBlue Rectangle fill blue color value
 * @param useFill Fill the rectangle, true/false
 */
void SSD1353::drawRectangle(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t lineRed, uint8_t lineGreen, uint8_t lineBlue, uint8_t fillRed, uint8_t fillGreen, uint8_t fillBlue, bool useFill) {
  enableFill(useFill);

  write(0x22, true);
  write(startX, false);
  write(startY, false);
  write(endX, false);
  write(endY, false);
  write(lineBlue, false);
  write(lineGreen, false);
  write(lineRed, false);
  write(fillBlue, false);
  write(fillGreen, false);
  write(fillRed, false);
  delay(2);
}

/**
 * @brief Draw a line on the display
 *
 * @param startX Starting point X coordinate
 * @param startY Starting point Y coordinate
 * @param endX Ending point X coordinate
 * @param endY Ending point Y coordinate
 * @param lineRed Line red color value
 * @param lineGreen Line green color  value
 * @param lineBlue Line blue color value
 */
void SSD1353::drawLine(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t lineRed, uint8_t lineGreen, uint8_t lineBlue) {
  write(0x21, true);
  write(startX, false);
  write(startY, false);
  write(endX, false);
  write(endY, false);
  write(lineBlue, false);
  write(lineGreen, false);
  write(lineRed, false);
}

/**
 * @brief Draw a dot on the display
 *
 * @param x Location X coordinate
 * @param y Location Y coordinate
 * @param valueRed Dot red color value
 * @param valueGreen Dot green color value
 * @param valueBlue Dot blue color value
 */
void SSD1353::drawDot(uint8_t x, uint8_t y, uint8_t valueRed, uint8_t valueGreen, uint8_t valueBlue) {
  drawLine(x, y, x, y, valueRed, valueGreen, valueBlue);
}

/**
 * @brief Fill display with given color
 *
 * @param valueRed Fill color red value
 * @param valueGreen Fill color green value
 * @param valueBlue Fill color blue value
 */
void SSD1353::fill(uint8_t valueRed, uint8_t valueGreen, uint8_t valueBlue) {
  drawRectangle(0, 0, 159, 127, valueRed, valueGreen, valueBlue, valueRed, valueGreen, valueBlue, true);
}

/**
 * @brief Clear the screen contents
 *
 */
void SSD1353::clear() {
  fill(0, 0, 0);
}

/**
 * @brief Print a character on the display
 *
 * @param x Character bottom left corner X coordinate
 * @param y Character bottom left corner Y coordinate
 * @param character Character to  be printed
 * @param valueRed Character red color value
 * @param valueGreen Character green color value
 * @param valueBlue Character blue color value
 */
void SSD1353::printchar(uint8_t x, uint8_t y, char character, uint8_t valueRed, uint8_t valueGreen, uint8_t valueBlue) {
  switch (character) {
    case 'A':
      drawLine(x, y, x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 2, x + 3, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case 'B':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 4, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case 'C':
      drawLine(x, y + 1, x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 1, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case 'D':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case 'E':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 4, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'F':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'G':
      drawLine(x, y + 1, x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 3, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case 'H':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'I':
      drawLine(x + 2, y, x + 2, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'J':
      drawDot(x, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 2, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 3, y + 1, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'K':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 2, y + 3, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 2, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 5, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'L':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 4, y, valueRed, valueGreen, valueBlue);
      break;

    case 'M':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 3, x + 2, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'N':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 4, x + 3, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'O':
      drawLine(x, y + 1, x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case 'P':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 4, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case 'Q':
      drawLine(x, y + 1, x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 2, x + 4, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case 'R':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 4, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 2, valueRed, valueGreen, valueBlue);
      break;

    case 'S':
      drawLine(x, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 4, x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'T':
      drawLine(x + 2, y, x + 2, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 6, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'U':
      drawLine(x, y + 1, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'V':
      drawLine(x, y + 2, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 1, x + 2, y, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 2, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'W':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 2, x + 2, y + 3, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'X':
      drawLine(x, y, x, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 5, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 2, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 5, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'Y':
      drawLine(x, y + 5, x, y + 6, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y, x + 2, y + 3, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 5, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'Z':
      drawLine(x, y, x + 4, y, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 1, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 6, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'a':
      drawLine(x, y + 1, x, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 2, x + 3, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 4, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case 'b':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 2, valueRed, valueGreen, valueBlue);
      break;

    case 'c':
      drawLine(x, y + 1, x, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 4, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 1, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case 'd':
      drawLine(x, y + 1, x, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'e':
      drawLine(x, y + 1, x, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 4, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 2, x + 4, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 4, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case 'f':
      drawLine(x + 1, y, x + 1, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 3, x + 2, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case 'g':
      drawDot(x, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 2, x + 3, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 4, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case 'h':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 2, valueRed, valueGreen, valueBlue);
      break;

    case 'i':
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 1, x + 2, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 2, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'j':
      drawDot(x, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 2, y, valueRed, valueGreen, valueBlue);
      drawDot(x + 2, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 3, y + 1, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'k':
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 2, x + 2, y + 2, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 1, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 4, valueRed, valueGreen, valueBlue);
      break;

    case 'l':
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 1, x + 2, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case 'm':
      drawLine(x, y, x, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 4, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y, x + 2, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case 'n':
      drawLine(x, y, x, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 4, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case 'o':
      drawLine(x, y + 1, x, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 4, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case 'p':
      drawLine(x + 1, y, x + 1, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 2, x + 3, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 4, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case 'q':
      drawLine(x + 3, y, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 2, x + 2, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 4, x + 2, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case 'r':
      drawLine(x, y, x, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 4, x + 4, y + 4, valueRed, valueGreen, valueBlue);
      break;

    case 's':
      drawLine(x, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawDot(x, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 2, x + 3, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 4, x + 4, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 1, valueRed, valueGreen, valueBlue);
      break;

    case 't':
      drawLine(x + 2, y + 1, x + 2, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 4, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 3, y, x + 4, y, valueRed, valueGreen, valueBlue);
      break;

    case 'u':
      drawLine(x, y + 1, x, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 2, y, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 4, valueRed, valueGreen, valueBlue);
      break;

    case 'v':
      drawLine(x, y + 2, x, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 1, x + 2, y, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 2, x + 4, y + 4, valueRed, valueGreen, valueBlue);
      break;

    case 'w':
      drawLine(x, y, x, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y, x + 2, y + 2, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 4, valueRed, valueGreen, valueBlue);
      break;

    case 'x':
      drawDot(x, y, valueRed, valueGreen, valueBlue);
      drawDot(x, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 1, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 3, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 1, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 4, valueRed, valueGreen, valueBlue);
      break;

    case 'y':
      drawLine(x, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 3, x, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 2, x + 3, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 4, valueRed, valueGreen, valueBlue);
      break;

    case 'z':
      drawLine(x, y, x + 4, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 1, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 4, x + 4, y + 4, valueRed, valueGreen, valueBlue);
      break;

    case '0':
      drawLine(x, y + 1, x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 2, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case '1':
      drawLine(x, y, x + 4, y, valueRed, valueGreen, valueBlue);
      drawDot(x, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 1, x + 2, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case '2':
      drawLine(x, y, x + 4, y, valueRed, valueGreen, valueBlue);
      drawDot(x, y + 1, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 4, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawDot(x, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case '3':
      drawDot(x, y + 1, valueRed, valueGreen, valueBlue);
      drawDot(x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 4, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case '4':
      drawLine(x, y + 2, x + 4, y + 2, valueRed, valueGreen, valueBlue);
      drawDot(x, y + 3, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 2, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 3, y, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case '5':
      drawDot(x, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 4, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 6, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case '6':
      drawLine(x, y + 1, x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 2, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case '7':
      drawLine(x, y + 6, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 3, y + 4, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y, x + 2, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case '8':
      drawLine(x, y + 1, x, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 4, x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 4, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case '9':
      drawDot(x, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 4, x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 1, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case '&':
      drawLine(x, y + 1, x, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 4, x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 2, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 4, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 4, x + 2, y + 5, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 6, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 2, valueRed, valueGreen, valueBlue);
      break;

    case '\'':
      drawLine(x + 2, y + 5, x + 2, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case '(':
      drawLine(x + 2, y + 1, x + 2, y + 5, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case ')':
      drawDot(x + 1, y, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 1, x + 2, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case '*':
      drawLine(x, y + 1, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 3, x + 4, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 5, x + 4, y + 1, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 1, x + 2, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case '+':
      drawLine(x, y + 3, x + 4, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 1, x + 2, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case '-':
      drawLine(x, y + 3, x + 4, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case '=':
      drawLine(x, y + 2, x + 4, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 4, x + 4, y + 4, valueRed, valueGreen, valueBlue);
      break;

    case '.':
      drawDot(x + 2, y, valueRed, valueGreen, valueBlue);
      break;

    case '!':
      drawDot(x + 2, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 2, x + 2, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case '"':
      drawLine(x + 1, y + 5, x + 1, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 3, y + 5, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case '#':
      drawLine(x, y + 2, x + 4, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x, y + 4, x + 4, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 1, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 3, y, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case '$':
      drawLine(x, y + 1, x + 3, y + 1, valueRed, valueGreen, valueBlue);
      drawDot(x, y + 4, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 5, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y, x + 2, y + 6, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 2, valueRed, valueGreen, valueBlue);
      break;

    case '%':
      drawLine(x, y + 1, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      drawRectangle(x, y + 5, x + 1, y + 6, valueRed, valueGreen, valueBlue, valueRed, valueGreen, valueBlue, false);
      drawRectangle(x + 3, y, x + 4, y + 1, valueRed, valueGreen, valueBlue, valueRed, valueGreen, valueBlue, false);
      break;

    case '^':
      drawLine(x, y + 4, x + 2, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 3, y + 5, x + 4, y + 4, valueRed, valueGreen, valueBlue);
      break;

    case ',':
      drawDot(x + 1, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 1, x + 2, y + 2, valueRed, valueGreen, valueBlue);
      break;

    case ':':
      drawLine(x + 2, y + 1, x + 2, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 4, x + 2, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case ';':
      drawLine(x + 2, y + 1, x + 2, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 4, x + 2, y + 5, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y, valueRed, valueGreen, valueBlue);
      break;

    case '?':
      drawDot(x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawDot(x + 2, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 2, x + 4, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case '@':
      drawLine(x, y + 1, x, y + 5, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y + 2, x + 2, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 2, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y + 3, x + 4, y + 5, valueRed, valueGreen, valueBlue);
      break;

    case '/':
      drawLine(x, y, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case '<':
      drawLine(x, y + 3, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 2, x + 3, y, valueRed, valueGreen, valueBlue);
      break;

    case '>':
      drawLine(x + 1, y, x + 4, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 4, valueRed, valueGreen, valueBlue);
      break;

    case '|':
      drawLine(x + 2, y, x + 2, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case '\\':
      drawLine(x, y + 6, x + 4, y, valueRed, valueGreen, valueBlue);
      break;

    case '[':
      drawLine(x + 2, y, x + 2, y + 6, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case ']':
      drawDot(x + 1, y, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 2, y, x + 2, y + 6, valueRed, valueGreen, valueBlue);
      break;

    case '{':
      drawLine(x + 1, y + 3, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 3, x + 3, y, valueRed, valueGreen, valueBlue);
      break;

    case '}':
      drawLine(x + 1, y, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case '_':
      drawLine(x, y, x + 4, y, valueRed, valueGreen, valueBlue);
      break;

    case '~':
      drawDot(x, y + 3, valueRed, valueGreen, valueBlue);
      drawDot(x + 1, y + 4, valueRed, valueGreen, valueBlue);
      drawDot(x + 2, y + 3, valueRed, valueGreen, valueBlue);
      drawDot(x + 3, y + 2, valueRed, valueGreen, valueBlue);
      drawDot(x + 4, y + 3, valueRed, valueGreen, valueBlue);
      break;

    case ' ':
      break;

    default:
      drawLine(x, y, x, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y, x + 3, y, valueRed, valueGreen, valueBlue);
      drawLine(x + 1, y + 6, x + 3, y + 6, valueRed, valueGreen, valueBlue);
      drawLine(x + 4, y, x + 4, y + 6, valueRed, valueGreen, valueBlue);
      break;
  }
}

/**
 * @brief Print a string on the display
 *
 * @param input String to be printed
 * @param x String bottom left corner X coordinate
 * @param y String bottom left corner Y coordinate
 * @param valueRed String color red value
 * @param valueGreen String color green value
 * @param valueBlue String color blue value
 */
void SSD1353::printstr(const char* input, uint8_t x, uint8_t y, uint8_t valueRed, uint8_t valueGreen, uint8_t valueBlue) {
  for (uint8_t i = 0; i < strlen(input); i++) {
    printchar((x + i * 6), y, input[i], valueRed, valueGreen, valueBlue);
  }
}
