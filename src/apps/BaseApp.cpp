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
#include "BaseApp.h"

BaseApp::BaseApp(HardwareSerial* hs, TFT_eSPI* tft) : _hs(hs), _tft(tft)
{
    _offscreen = new TFT_eSprite(tft);  // Declare Sprite object "spr" with pointer to "tft" object
    _offscreen->createSprite(tft->width(), tft->height());
}

BaseApp::~BaseApp()
{
    cleanup();
    _offscreen->deleteSprite();
    //delete _offscreen;
}

// standard apps only need runlevel 1, if you need higher, override this function
RunLevel BaseApp::runLevelNeeded()
{
    return RL1;
};

void BaseApp::handleEvents(EventHandler* eh)
{
    // call event handler without param for convenience
    handleEvents();
}

void BaseApp::handleEvents(){}

void BaseApp::cleanup()
{
    // do nothing
}

void BaseApp::paintFrame()
{
    paintFrameInternal();

    // blit the offscreen to the screen
    if(_offscreen != nullptr)
    {
        _offscreen->pushSprite(0, 0);
    }
}

void BaseApp::paintFrameInternal()
{
    fillScreen(TFT_BLACK);

    float p = 2 * sin(d);
    float q = 2 * sin(-d);

    for (byte k = 1; k < 16; k++) {
        // some calculations are used multiple times
        // so better caclutate them once and reuse
        // the results.
        float r = sin(d + (k * 0.4));
        float t = sin(d + 3 + (k * 0.4));

        // First lets draw the top 12 balls
        fillCircle(k * 8 + p, 12 + (r * 10), 2, TFT_GREENYELLOW);
        // and now the bottom 12 balls
        fillCircle(k * 8 + q, 12 + (t * 10), 2, TFT_GREENYELLOW);

        // now we draw a line from the top ball to the bottom ball
        drawLine(k * 8 + p, 10 + (r * 10), k * 8 + q, (10 + t * 10), TFT_GREENYELLOW);

        // and finally we need a line from ball to ball on the top part
        if(k < 15) {
            drawLine((k * 8 + p), 12 + (r * 10), 6 + (k * 8 + p), (12 + sin(d + ((k + 1) * 0.4)) * 10), TFT_GREENYELLOW);
        }

        // and the bottom part
        if(k < 15) {
            drawLine((k * 8 + q), 12 + (t * 10), 6 + (k * 8 + q), (12 + sin(d + 3 + ((k + 1) * 0.4)) * 10), TFT_GREENYELLOW);
        }
    }

    // float d goes 1 step further on the sine evolution
    d = d + 0.10;
}

bool BaseApp::getEnforceFramerate()
{
    return true;
}

bool BaseApp::canWeGoToSleep()
{
    return true;
}

bool BaseApp::canSwitchAway()
{
    return true;
}

// Graphics

void BaseApp::clear()
{
    fillScreen(TFT_BLACK);
}

void BaseApp::drawPixel(int x, int y, uint8_t color)
{
    _offscreen->drawPixel(x, y, color);
}

/*uint8_t BaseApp::getPixel(uint8_t x, uint8_t y)
{
    uint8_t row = y / 8;
    uint8_t bit_position = y % 8;
    return (sBuffer[(row*SCREEN_WIDTH) + x] & _BV(bit_position)) >> bit_position;
}*/

void BaseApp::drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
    _offscreen->drawCircle(x0, y0, r, color);
}

void BaseApp::fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
    _offscreen->drawCircle(x0, y0, r, color);
}

void BaseApp::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color)
{
    _offscreen->drawLine(x0, y0, x1, y1, color);
}

void BaseApp::drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
    _offscreen->drawRect(x, y, w, h, color);
}

void BaseApp::fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
    _offscreen->drawRect(x, y, w, h, color);
}

void BaseApp::fillScreen(uint8_t color)
{
    _offscreen->fillScreen(color);
}

void BaseApp::drawRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
    _offscreen->drawRoundRect(x, y, w, h, r, color);
}

void BaseApp::fillRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
    _offscreen->drawRoundRect(x, y, w, h, r, color);
}

void BaseApp::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

}

void BaseApp::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

}

void BaseApp::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color)
{

}

void BaseApp::drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size)
{

}

