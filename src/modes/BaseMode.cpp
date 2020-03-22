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
#include "BaseMode.h"

BaseMode::BaseMode(HardwareSerial* hs, TFT_eSPI* tft) : _hs(hs), _tft(tft)
{
    _offscreen = new TFT_eSprite(tft);  // Declare Sprite object "spr" with pointer to "tft" object
    _offscreen->createSprite(tft->width(), tft->height());
}

BaseMode::~BaseMode()
{
    cleanup();
    delete _offscreen;
}

void BaseMode::handleEvents()
{
    // do nothing
}

void BaseMode::cleanup()
{
    // do nothing
}

void BaseMode::paintFrame()
{
    paintFrameInternal();

    // blit the offscreen to the screen
    if(_offscreen != nullptr)
    {
        _offscreen->pushSprite(0, 0);
    }
}

void BaseMode::paintFrameInternal()
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

bool BaseMode::getEnforceFramerate()
{
    return true;
}

bool BaseMode::canWeGoToSleep()
{
    return true;
}

// Graphics

void BaseMode::clear()
{
    fillScreen(TFT_BLACK);
}

void BaseMode::drawPixel(int x, int y, uint8_t color)
{
    _offscreen->drawPixel(x, y, color);
}

/*uint8_t BaseMode::getPixel(uint8_t x, uint8_t y)
{
    uint8_t row = y / 8;
    uint8_t bit_position = y % 8;
    return (sBuffer[(row*SCREEN_WIDTH) + x] & _BV(bit_position)) >> bit_position;
}*/

void BaseMode::drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
    _offscreen->drawCircle(x0, y0, r, color);
}

void BaseMode::fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
    _offscreen->drawCircle(x0, y0, r, color);
}

void BaseMode::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color)
{
    _offscreen->drawLine(x0, y0, x1, y1, color);
}

void BaseMode::drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
    _offscreen->drawRect(x, y, w, h, color);
}

void BaseMode::fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
    _offscreen->drawRect(x, y, w, h, color);
}

void BaseMode::fillScreen(uint8_t color)
{
    _offscreen->fillScreen(color);
}

void BaseMode::drawRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
    _offscreen->drawRoundRect(x, y, w, h, r, color);
}

void BaseMode::fillRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
    _offscreen->drawRoundRect(x, y, w, h, r, color);
}

void BaseMode::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

}

void BaseMode::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

}

void BaseMode::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color)
{

}

void BaseMode::drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size)
{

}

