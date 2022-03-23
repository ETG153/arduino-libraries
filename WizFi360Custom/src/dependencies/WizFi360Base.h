/*
  WizFi360Base.h -
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

#ifndef WIZFI360BASE_H
#define WIZFI360BASE_H

#include <stdint.h>

//#define DEBUG
#ifdef DEBUG
#include <HardwareSerial.h>
#endif

class WizFi360Drv {
  public:
  uint8_t init(class Stream* serial, uint8_t rst_pin);
  bool sendCMD(char* command);
  void getResponse(char* command, char* responsebuff);

  private:
  Stream* _serial;
  uint8_t _rst_pin;

#ifdef DEBUG
  Stream* debug = &Serial;
#endif
};

#endif
