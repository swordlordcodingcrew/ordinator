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
#include "AppAbout.h"

AppAbout::AppAbout(HardwareSerial* hws, TFT_eSPI* s) : BaseApp (hws, s)
{
    _tft->fillScreen(TFT_BLACK);

    delete _offscreen;

    // create an additional screen in landscape mode
    _textScreen = new TFT_eSprite(_tft);
    _textScreen->createSprite(_tft->height(), _tft->width());
}

AppAbout::~AppAbout()
{
    _textScreen->deleteSprite();
}

void AppAbout::handleEvents()
{
    frameCount++;

    if(frameCount < everyXFrame)
    {
        return;
    }

    // run this only if its time to increase
    position++;

    if(position >= scrolltext.length())
    {
        position = 0;
    }
}

void AppAbout::paintFrameInternal()
{
    // slow down the display
    // yeah, this is an ugly hack, but you know, a few days before deadline...
    if(frameCount < everyXFrame)
    {
        return;
    }
    else
    {
        frameCount = 0;
    }

    _textScreen->fillScreen(TFT_BLACK);
    _textScreen->setTextColor(TFT_GREENYELLOW);
    _textScreen->setTextDatum(MC_DATUM);
    _textScreen->setTextFont(4);

    // wobbly text
    for (int8_t j = 0; j < 13; j++)
    {
        if((position + j) < scrolltext.length())
        {
            _textScreen->drawString(scrolltext.substr(position + j, 1).c_str(), (j * 10), 14 - sin(d + (j * 0.4)) * 5);

            // For Debugging Purposes...
            //_hs->print("x: ");
            //_hs->print((j * 10));
            //_hs->print("y: ");
            //_hs->print(14 - sin(d + (j * 0.4)) * 5);
            //_hs->print("c: ");
            //_hs->println(scrolltext.substr(position + j, 1).c_str());
        }
    }

    // float d goes 1 step further on the sine evolution
    d = d + 0.10;

    _textScreen->pushRotated(180);
}




