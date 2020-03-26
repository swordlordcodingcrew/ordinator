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
#ifndef ORDINATOR_MODECLOCK_H
#define ORDINATOR_MODECLOCK_H

#include "../EventHandler.h"
#include "BaseMode.h"
#include <pcf8563.h>
#include <HardwareManager.h>
#include "watchface/kaerste.h"

class ModeClock : public BaseMode
{
public:
    ModeClock(HardwareSerial* hws, TFT_eSPI* s, HardwareManager* hwm);

    void handleEvents(EventHandler* eh);
    void paintFrameInternal();

protected:


private:

    HardwareManager* _hwm = nullptr;

};

#endif // ORDINATOR_MODECLOCK_H
