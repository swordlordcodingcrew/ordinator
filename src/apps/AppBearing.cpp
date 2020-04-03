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
#include "AppBearing.h"

AppBearing::AppBearing(HardwareSerial* hws, TFT_eSPI* s, HardwareManager* hwm) : BaseApp (hws, s), _hwm(hwm)
{
    _offscreen->fillScreen(TFT_YELLOW);
}

// standard apps only need runlevel RL1, if you need higher, override this function
RunLevel AppBearing::runLevelNeeded()
{
    return RL3;
};


void AppBearing::handleEvents()
{

}

void AppBearing::paintFrameInternal()
{
    int16_t bearing = _hwm->getBearing();
    char bearingText[5] = "---";
    if (bearing >= 0)
    {
        sprintf(bearingText, "%03d", bearing);
    }

    _offscreen->setTextColor(TFT_BLACK, TFT_YELLOW);
    _offscreen->drawString(bearingText, 6, 20, 4);
}




