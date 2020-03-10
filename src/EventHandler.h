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
#include <EasyButton.h>
#include "hal.hpp"

class EventHandler
{
    public:
        EventHandler(HardwareSerial* hws, EasyButton* btn);

        void handleLoop();

        void shortPress();
        void longPress();

    protected:


    private:

        EasyButton tp_button = EasyButton(TP_PIN_PIN, 80, true, false);
        uint32_t time_out = millis();
        uint16_t max_time_out = 15000;

        HardwareSerial* hs = nullptr;
        EasyButton* _btn = nullptr;
};

#endif // ORDINATOR_EVENTHANDLER_H
