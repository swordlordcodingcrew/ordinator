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
#include "DisplayManager.h"

DisplayManager::DisplayManager(HardwareSerial* hs, ModeManager* mm, TFT_eSPI* tft) : _tft(tft), _mm(mm), _hs(hs)
{
    // frame management
    setFrameRate(60);
    frameCount = 0;
    nextFrameStart = 0;
    post_render = false;
    // init not necessary, will be reset after first use
    // lastFrameStart
    // lastFrameDurationMs
}

void DisplayManager::begin()
{
    _tft->init();
    _tft->setRotation(0); // 80x160
    _tft->setSwapBytes(true);
    _tft->fillScreen(TFT_BLACK);
    ledcSetup(0, 5000, 8);
    ledcAttachPin(TFT_BL, 0);
    ledcWrite(0, 185);
}

void DisplayManager::showBootLogo()
{
    _tft->pushImage(0, 0, 80, 160, shield);

    delay(2000);
}

//Frame management

void DisplayManager::setFrameRate(uint8_t rate)
{
    frameRate = rate;
    eachFrameMillis = 1000 / rate;
}

bool DisplayManager::everyXFrames(uint8_t frames)
{
    return frameCount % frames == 0;
}

bool DisplayManager::nextFrame()
{
    long now = millis();
    //uint8_t remaining;

    // post render
    if (post_render) {
        lastFrameDurationMs = now - lastFrameStart;
        frameCount++;
        post_render = false;
    }

    // if it's not time for the next frame yet
    if (now < nextFrameStart) {
        // remaining = nextFrameStart - now;
        // if we have more than 1ms to spare, lets sleep
        // we should be woken up by timer0 every 1ms, so this should be ok
        //if (remaining > 1)
        //    idle();
        return false;
    }

    // pre-render

    // technically next frame should be last frame + each frame but if we're
    // running a slow render we would constantly be behind the clock
    // keep an eye on this and see how it works.  If it works well the
    // lastFrameStart variable could be eliminated completely
    nextFrameStart = now + eachFrameMillis;
    lastFrameStart = now;
    post_render = true;

    return post_render;
}

// TODO fixme this is wrong, display manager cant know module...
void DisplayManager::handleFrame()
{
    BaseMode* m = _mm->getCurrentModeObject();

    //_hs->println("handle events");
    m->handleEvents();

    //_hs->println("paint frame");
    m->paintFrame();
}

void DisplayManager::commenceSleep()
{
    _tft->fillScreen(TFT_BLACK);
    _tft->writecommand(ST7735_SWRESET);
    delay(100);
    _tft->writecommand(ST7735_SLPIN);
    delay(150);
    _tft->writecommand(ST7735_DISPOFF);
}
