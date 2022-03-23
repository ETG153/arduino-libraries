/*
  WizFi360Custom.h -
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

#ifndef WIZFI360CUSTOM_H
#define WIZFI360CUSTOM_H

#include <stdint.h>

//#define DEBUG
#ifdef DEBUG
#include <HardwareSerial.h>
#endif

#define WIZFI_MODE_STATION (uint8_t)1
#define WIZFI_MODE_SOFTAP  (uint8_t)2
#define WIZFI_MODE_BOTH    (uint8_t)3

class WizFi360 {
  public:
  uint8_t init(class Stream* serial, uint8_t rst_pin);
  uint8_t setMode(uint8_t mode);
  uint8_t connectWifi(const char* SSID, const char* password);
  uint8_t disconnectWifi(void);

  private:
  uint8_t _workingMode = 0;
  bool _wifiConnected = false;

#ifdef DEBUG
  Stream* debug = &Serial;
#endif
};

#endif
