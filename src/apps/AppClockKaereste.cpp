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
#include <sstream>
#include "AppClockKaereste.h"

AppClockKaereste::AppClockKaereste(HardwareSerial* hws, TFT_eSPI* s, HardwareManager* hwm) : BaseApp (hws, s), _hwm(hwm)
{
    _offscreen->fillScreen(TFT_BLACK);
}

void AppClockKaereste::handleEvents(EventHandler* eh)
{
    if(eh->isButtonJustReleased() && eh->buttonWasDownForAtLeast(2))
    {
        _tft->fillScreen(TFT_VIOLET);
        _hwm->adjustRTC();
    }
}

void AppClockKaereste::paintFrameInternal()
{
    RTC_Date now = _hwm->getCurrentTime();

    _offscreen->pushImage(0, 0, 80, 160, kaerste);

    uint8_t xpos = 6;
    uint8_t ypos = 12;
    _offscreen->setTextDatum(TL_DATUM);
    _offscreen->setTextSize(1);
    _offscreen->setTextFont(4);
    _offscreen->setTextColor(TFT_BLACK);

    if (now.hour < 10)
    {
        xpos += _offscreen->drawChar('0', xpos, ypos);
    }

    xpos += _offscreen->drawNumber(now.hour, xpos, ypos);
    xpos += _offscreen->drawChar(':', xpos, ypos);

    if (now.minute < 10)
    {
        xpos += _offscreen->drawChar('0', xpos, ypos);
    }

    _offscreen->drawNumber(now.minute, xpos, ypos);

    std::ostringstream fd;
    fd << now.day << "." << now.month << "." << now.year;

    _offscreen->setTextFont(2);
    xpos = 4;
    ypos = 50;
    xpos += _offscreen->drawString(fd.str().c_str(), xpos, ypos);
}




