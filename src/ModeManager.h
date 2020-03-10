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
#ifndef ORDINATOR_MODEMANAGER_H
#define ORDINATOR_MODEMANAGER_H

#include <Arduino.h>
#include "EventHandler.h"
#include "modes/BaseMode.h"
#include "modes/ModeAbout.h"

class ModeManager
{
public:
    ModeManager(HardwareSerial* hs, TFT_eSPI* tft);

    void checkEvents();

    uint8_t getCurrentMode();
    BaseMode* getCurrentModeObject();

    bool moduleWantsEnforcedFramerate();

    void setMode(uint8_t newMode, uint8_t oldMode, bool storeMode = true);

protected:


private:
    uint8_t currentMode = 0;
    BaseMode* currentModeObject = nullptr;

    HardwareSerial* _hs = nullptr;
    TFT_eSPI* _tft = nullptr;
};

#endif // ORDINATOR_MODEMANAGER_H