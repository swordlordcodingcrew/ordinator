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

#include "Ordinator.h"

Ordinator::Ordinator(HardwareSerial* hws)
{
    hs = hws;
}

void Ordinator::setup(EasyButton* btn)
{
    Serial.begin(115200);
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(400000);
    /*
     * eh = new EventHandler(&Serial);
    eh->poll();

    // initialise the mode manager and implicitly the first mode to be run
    _mm = new ModeManager(eh, c, &u8g2, &Serial);

    // initialise the display
    dm = new DisplayManager(_mm, &u8g2, &Serial);
    dm->begin();
     */

    eh = new EventHandler(&Serial, btn);

    // initialise the mode manager and implicitly the first mode to be run
    mm = new ModeManager(&Serial, &_tft);

    // initialise the display
    dm = new DisplayManager(&Serial, mm, &_tft);
    dm->begin();
    dm->showBootLogo();

    // initialise the hardware manager
    hwm = new HardwareManager(&Serial);

    btStop();

    Serial.println("Welcome to your Ordinator. All systems are up.");
}

void Ordinator::loop()
{
    // only enforce framerate if the module wants it enforced
    if(mm->moduleWantsEnforcedFramerate() && !dm->nextFrame())
    {
        return;
    }

    // for debugging purposes, print when heap changes
    uint32_t freeHeap = ESP.getFreeHeap();
    if(freeHeap != lastFreeHeap)
    {
        Serial.print("Free Heap: ");
        Serial.println(freeHeap);

        lastFreeHeap = freeHeap;
    }

    // poll fills the bitmask with the current button state. no poll, no updates.
    //eh->poll();
    eh->handleLoop();

    // have the module check its events
    mm->checkEvents();

    // TODO update battery charge status
    //updateBatteryChargeStatus();

    // have the module paint its UI
    dm->handleFrame();
}
