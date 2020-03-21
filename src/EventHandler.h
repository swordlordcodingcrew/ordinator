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
#ifndef ORDINATOR_EVENTHANDLER_H
#define ORDINATOR_EVENTHANDLER_H

#include <Arduino.h>
#include "hal.hpp"

class EventHandler
{
    public:
        EventHandler(HardwareSerial* hws);

        void poll();

        bool isButtonPressed();
        bool isButtonJustPressed();
        bool isButtonReleased();
        bool isButtonJustReleased();

        bool timeoutForSleepReached();

        void shortPress();
        void longPress();

    protected:


    private:

        const uint16_t _maxTimeOut = 15000;
        uint32_t _lastEventAt = 0;

        bool isPressed = false;
        bool lastPressed1 = false;
        bool lastPressed2 = false;
        bool lastPressed3 = false;
        bool lastPressed4 = false;
        bool lastPressed5 = false;

        HardwareSerial* hs = nullptr;
};

#endif // ORDINATOR_EVENTHANDLER_H
