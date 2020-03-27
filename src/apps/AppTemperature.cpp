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
#include "AppTemperature.h"

AppTemperature::AppTemperature(HardwareSerial* hws, TFT_eSPI* s, HardwareManager* hwm) : BaseApp (hws, s), _hwm(hwm)
{

}

AppTemperature::~AppTemperature() {}

void AppTemperature::handleEvents()
{

}

void AppTemperature::paintFrameInternal()
{
    uint8_t xpos = 6;
    uint8_t ypos = 50;
    uint16_t colonX = 0;

    _offscreen->setTextDatum(TL_DATUM);
    _offscreen->setTextColor(TFT_GREENYELLOW);

    float temperature = _hwm->getTemperature();
    char t[8] = "";
    sprintf(t, "%.1fC", temperature);

    xpos += _offscreen->drawString(t, xpos, ypos, 7);
}




