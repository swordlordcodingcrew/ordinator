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
#include "DisplayManager.h"
#include "modes/BaseMode.h"
#include "modes/ModeAbout.h"
#include "modes/ModeClock.h"
#include "modes/ModeLogo.h"
#include "modes/ModeOTA.h"
#include "modes/ModeBearing.h"
#include "HardwareManager.h"

class ModeManager
{
public:
    ModeManager(HardwareSerial* hs, DisplayManager* dm, EventHandler* eh, HardwareManager* hwm);

    void checkEvents();

    uint8_t getCurrentMode();
    BaseMode* getCurrentModeObject();

    bool moduleWantsEnforcedFramerate();

    void handleLoop();

    bool canWeGoToSleep();

    void setMode(uint8_t newMode, uint8_t oldMode, bool storeMode = true);

protected:

    void switchToNextMode();


private:
    uint8_t _currentMode = 0;
    BaseMode* _currentModeObject = nullptr;

    HardwareSerial* _hs = nullptr;
    DisplayManager* _dm = nullptr;
    EventHandler* _eh = nullptr;
    HardwareManager* _hwm = nullptr;
};

#endif // ORDINATOR_MODEMANAGER_H
