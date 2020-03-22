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
#include "ModeClock.h"

ModeClock::ModeClock(HardwareSerial* hws, TFT_eSPI* s, HardwareManager* hwm) : BaseMode (hws, s), _hwm(hwm)
{
    _offscreen->fillScreen(TFT_BLACK);
}

void ModeClock::handleEvents()
{

}

void ModeClock::paintFrameInternal()
{
    RTC_Date now = _hwm->getClockTime();

    uint8_t xpos = 6;
    uint8_t ypos = 6;
    uint16_t colonX = 0;
    _offscreen->setTextDatum(TL_DATUM);
    _offscreen->setTextColor(0x0821, TFT_BLACK);
    _offscreen->drawString("88", xpos, ypos, 7);
    _offscreen->setTextColor(TFT_GREENYELLOW);

    if (now.hour < 10)
    {
        xpos += _offscreen->drawChar('0', xpos, ypos, 7);
    }

    xpos += _offscreen->drawNumber(now.hour, xpos, ypos, 7);
    //xpos += displayColon(xpos, true, utc);

    ypos += 56;
    xpos = 6;

    _offscreen->setTextColor(0x0821, TFT_BLACK);
    _offscreen->drawString("88", xpos, ypos, 7);
    _offscreen->setTextColor(TFT_GREENYELLOW);

    if (now.minute < 10)
    {
        xpos += _offscreen->drawChar('0', xpos, ypos, 7);
    }

    _offscreen->drawNumber(now.minute, xpos, ypos, 7);
}




