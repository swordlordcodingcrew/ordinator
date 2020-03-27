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

#include <string>
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
        uint32_t getButtonDownTime();
        bool buttonWasDownForAtLeast(uint8_t seconds);

        bool timeoutForSleepReached();
        void resetLastEventTimestamp();

    protected:


    private:

        // go to sleep after 10 seconds (if allowed)
        const uint16_t _MAX_TIMEOUT = 5000;

        uint64_t _lastEventAt = 0;

        uint64_t _buttonDownAt = 0;
        uint32_t _lastButtonDownDuration = 0;

        bool isPressed = false;
        bool lastPressed1 = false;
        bool lastPressed2 = false;
        bool lastPressed3 = false;
        bool lastPressed4 = false;
        bool lastPressed5 = false;

        HardwareSerial* _hs = nullptr;
};

#endif // ORDINATOR_EVENTHANDLER_H
