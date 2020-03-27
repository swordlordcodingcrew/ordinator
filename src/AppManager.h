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
#ifndef ORDINATOR_APPMANAGER_H
#define ORDINATOR_APPMANAGER_H

#include <Arduino.h>
#include "EventHandler.h"
#include "DisplayManager.h"
#include "apps/BaseApp.h"
#include "apps/AppAbout.h"
#include "apps/AppClockKaereste.h"
#include "apps/AppClockDigital.h"
#include "apps/AppLogo.h"
#include "apps/AppOTA.h"
#include "apps/AppBearing.h"
#include "apps/AppBattery.h"
#include "apps/AppTemperature.h"
#include "HardwareManager.h"

class AppManager
{
public:
    AppManager(HardwareSerial* hs, DisplayManager* dm, EventHandler* eh, HardwareManager* hwm);

    void checkEvents();

    uint8_t getCurrentAppID();
    BaseApp* getCurrentApp();

    bool appWantsEnforcedFramerate();

    void handleLoop();

    bool canWeGoToSleep();

    void setMode(uint8_t newMode, uint8_t oldMode, bool storeMode = true);

protected:

    void switchToNextMode();


private:
    uint8_t _currentAppID = 0;
    BaseApp* _currentApp = nullptr;

    HardwareSerial* _hs = nullptr;
    DisplayManager* _dm = nullptr;
    EventHandler* _eh = nullptr;
    HardwareManager* _hwm = nullptr;
};

#endif //ORDINATOR_APPMANAGER_H
