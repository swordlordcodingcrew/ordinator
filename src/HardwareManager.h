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
#ifndef ORDINATOR_HARDWAREMANAGER_H
#define ORDINATOR_HARDWAREMANAGER_H

#include <HardwareSerial.h>

class HardwareManager
{
public:
    HardwareManager(HardwareSerial* hws);

    void initClock();
    void tftInit();
    void activateWifi();
    void deactivateWifi();
    void btStop();
    void setupADC();
    void initMPU();
    void initButton();
    void setupBattery();

protected:

private:

    HardwareSerial* hs = nullptr;
};

#endif //ORDINATOR_HARDWAREMANAGER_H
