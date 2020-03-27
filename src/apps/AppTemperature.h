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
#ifndef ORDINATOR_APPTEMPERATURE_H
#define ORDINATOR_APPTEMPERATURE_H

#include <HardwareManager.h>
#include "../EventHandler.h"
#include "BaseApp.h"

class AppTemperature : public BaseApp
{
public:
    AppTemperature(HardwareSerial* hws, TFT_eSPI* s, HardwareManager* hwm);
    ~AppTemperature();

    void handleEvents();
    void paintFrameInternal();

protected:

private:
    HardwareManager* _hwm = nullptr;

};

#endif //ORDINATOR_APPTEMPERATURE_H
