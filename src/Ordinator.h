/*-----------------------------------------------------------------------------
 **
 ** - Ordinator -
 **
 ** Copyright 2020 by SwordLord - the coding crew and the contributing authors
 **
 ** This program is free software; you can redistribute it and/or modify it
 ** under the terms of the GNU Affero General Public License as published by the
 ** Free Software Foundation, either version 3 of the License, or (at your option)
 ** any later version.
 **
 ** This program is distributed in the hope that it will be useful, but WITHOUT
 ** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 ** FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License
 ** for more details.
 **
 ** You should have received a copy of the GNU Affero General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **
 ** -----------------------------------------------------------------------------*/
#ifndef ORDINATOR_ORDINATOR_H
#define ORDINATOR_ORDINATOR_H

#include <HardwareSerial.h>
#include <Arduino.h>
#include "global.h"
#include <Wire.h>
#include "hal.hpp"
#include "DisplayManager.h"
#include "EventHandler.h"
#include "ModeManager.h"
#include "HardwareManager.h"

class Ordinator
{
public:
    Ordinator(HardwareSerial* hws);

    void setup();
    void loop();

protected:
    void sleep();

private:

    DisplayManager* _dm = nullptr;
    EventHandler* _eh = nullptr;
    ModeManager* _mm = nullptr;
    HardwareManager* _hwm = nullptr;
    HardwareSerial* _hs = nullptr;
    //Config* c = NULL;

    TFT_eSPI _tft = TFT_eSPI();

    uint32_t _lastFreeHeap = 0;
};

#endif //ORDINATOR_ORDINATOR_H
