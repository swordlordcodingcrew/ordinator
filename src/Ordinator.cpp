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

Ordinator::Ordinator(HardwareSerial* hws) : _hs(hws)
{

}

void Ordinator::setup()
{
    Serial.begin(115200);
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(400000);

    // TODO decie if we should redirect log to SPIFFS
    //esp_log_set_vprintf();
    log_i("Welcome to your Ordinator. Booting up.");

    // initialise the display
    _dm = new DisplayManager(&Serial, &_tft);
    _dm->showBootLogo();

    _eh = new EventHandler(&Serial);
    _eh->poll();

    // initialise the hardware manager
    _hwm = new HardwareManager(&Serial);

    log_i("Initialise AppManager");
    // initialise the mode manager and implicitly the first mode to be run
    _mm = new AppManager(&Serial, _dm, _eh, _hwm);

    // TODO fixme
    btStop();

    _eh->resetLastEventTimestamp();
}

void Ordinator::loop()
{
    // only enforce framerate if the module wants it enforced
    if(_mm->appWantsEnforcedFramerate() && !_dm->nextFrame())
    {
        return;
    }

    // for debugging purposes, print when heap changes
    uint32_t freeHeap = ESP.getFreeHeap();
    if(freeHeap != _lastFreeHeap)
    {
        log_i("Free Heap: %d", freeHeap);

        _lastFreeHeap = freeHeap;
    }

    // poll fills the bitmask with the current button state. no poll, no updates.
    _eh->poll();

    // have the module check its events
    _mm->checkEvents();

    // go to sleep if timeout reached and active module is ok with it
    if (_eh->timeoutForSleepReached() && !_eh->isButtonPressed() && _mm->canWeGoToSleep() && !_hwm->isBatteryCharging())
    {
        sleep();
    }

    // have the LED light up, if Device is charged..
    _hwm->updateChargeLED();

    // have the module paint its UI
    _mm->handleLoop();
}

void Ordinator::sleep()
{
    _dm->commenceSleep();
    _hwm->switchToRunLevel(RL0);
}
