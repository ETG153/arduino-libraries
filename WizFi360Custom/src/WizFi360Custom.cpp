/*
  WizFi360Custom.cpp -
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

#include "WizFi360Custom.h"
#include "dependencies/WizFi360Base.h"
#include <Arduino.h>

WizFi360Drv drv;

/**
 * @brief Library init function
 *
 * @param serial Pointer to serial port used by module
 * @return uint8_t Init exit code
 * 0 Module response received & OK
 * 1 Module not present
 * 2 Invalid response
 */
uint8_t WizFi360::init(class Stream* serial, uint8_t rst_pin) {
  return drv.init(serial, rst_pin);
} // init

/**
 * @brief Set module working mode - See WizFi360 user manual for options
 *
 * @param mode Mode to set module to
 * @return uint8_t Exit code
 * 0 - Mode set successfully
 * 1 - Invalid mode code
 * 2 - Command execution error
 */
uint8_t WizFi360::setMode(uint8_t mode) {
  char buff[14];

  if (mode < 1 || mode > 3)
    return 1;

  sprintf(buff, "AT+CWMODE=%d\r\n", mode);
  if (drv.sendCMD(buff)) {
    return 2;
  } else {
    _workingMode = mode;
    return 0;
  }
} // setMode

/**
 * @brief Connect to given WiFi network
 *
 * @param SSID SSID of target network
 * @param password Access point's password
 * @return uint8_t Command return code
 * 0 - Connected successfully
 * 1 - Connection timeout.
 * 2 - Wrong password.
 * 3 - Cannot find the target AP.
 * 4 - Connection failed.
 * 5 - Module not in Station mode
 * 6 - Unknown error
 */
uint8_t WizFi360::connectWifi(const char* SSID, const char* password) {
  char buff[113];
  static char response[52];

  // Check that the module is in Station mode
  if (_workingMode != WIZFI_MODE_STATION) {
    return 5;
  }

  sprintf(buff, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, password);
  drv.getResponse(buff, response);
  if (strstr(response, "OK") != nullptr) {
    _wifiConnected = true;
    return 0;

  } else {
    _wifiConnected = false;

    char* errcodeptr = strchr(response, ':');
    if (errcodeptr != nullptr)
      return *(errcodeptr + 1);
    else
      return 6;
  }
} // connectWifi

/**
 * @brief Disconnect from current wifi network
 *
 * @return uint8_t Command return code
 * 0 - Disconnect OK
 * 1 - Error
 */
uint8_t WizFi360::disconnectWifi(void) {
  char buff[] = "AT+CWQAP\r\n";
  if (drv.sendCMD(buff) == 0) {
    _wifiConnected = false;
    return 0;
  } else {
    return 1;
  }
} // disconnectWifi