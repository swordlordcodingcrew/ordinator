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

#include "HardwareManager.h"

HardwareManager::HardwareManager(HardwareSerial* hws) : hs(hws)
{

}


void HardwareManager::initClock(){}
void HardwareManager::tftInit(){} // too late, display manager solves this
void HardwareManager::activateWifi(){}
void HardwareManager::deactivateWifi(){}
void HardwareManager::btStop(){}
void HardwareManager::setupADC(){}
void HardwareManager::initMPU(){}
void HardwareManager::initButton(){} // nopes, event manager handles this
void HardwareManager::setupBattery(){}
