/*
  WizFi360Base.cpp -
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

#include "WizFi360Base.h"
#include <Arduino.h>

/**
 * @brief Module init function
 *
 * @param serial Pointer to serial port used by module
 * @return 0 Module response received & OK
 * @return 1 Module not present
 * @return 2 Invalid response
 */
uint8_t WizFi360Drv::init(class Stream* serial, uint8_t rst_pin) {
  char buff[4];
  _serial = serial;

  _rst_pin = rst_pin;
  pinMode(_rst_pin, OUTPUT);
  digitalWrite(_rst_pin, LOW);
  delay(1);
  digitalWrite(_rst_pin, HIGH);

  uint32_t startTime = millis();
  while (!_serial->available()) {     // Start listening for "ready" from module
    if (millis() - startTime > 10000) // Timeout of 10 s
      return 1;                       // Nothing heard, module likely not present
  }
  delay(1);

  _serial->flush();
  _serial->print("ATE0\r\n");        // Echo off
  _serial->print("AT+CWAUTOCONN=0"); // WiFi autoconnect off

  while (!_serial->available()) // Wait for response
    ;
  delay(10);

  for (uint8_t i = 0; _serial->available(); i++) { // Read the last 4 bytes of response
    buff[0] = buff[1];
    buff[1] = buff[2];
    buff[2] = buff[3];
    buff[3] = _serial->read();
  }

  if (buff[0] == 'O' && buff[1] == 'K')
    return 0;
  else
    return 2;
} // init

/**
 * @brief Sends given AT command to module
 *
 * @param command Command to be sent to module
 * @return true - Error occured; false - Command executed successfully
 */
bool WizFi360Drv::sendCMD(char* command) {
  char buff[2];
  _serial->print(command);
  while (!_serial->available())
    ;
  delay(10);

#ifdef DEBUG
  while (_serial->available()) {
    debug->write(_serial->read());
  }
  return 0;
#else
  //  Discard bytes until only 4 remain
  while (_serial->available() > 4) {
    _serial->read();
  }

  //  Look for OK response
  for (uint8_t i = 0; i < 2; i++) {
    buff[i] = _serial->read();
  }
  _serial->flush();
  if (buff[0] == 'O' && buff[1] == 'K') {
    return 0;
  } else {
    return 1;
  }
#endif // DEBUG
} // sendCMD

/**
 * @brief Send command to module and read the response
 *
 * @param command Command to be sent to module
 * @param responsebuff Pointer to buffer for the response
 */
void WizFi360Drv::getResponse(char* command, char* responsebuff) {
  _serial->print(command);         // Send command
  while (_serial->available() < 6) // Wait for response
    ;
  delay(10);

  for (uint16_t i = 0; _serial->available() > 0; i++) {
    responsebuff[i] = _serial->read();
  }
} // getResponse
