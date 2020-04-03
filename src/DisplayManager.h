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
#ifndef ORDINATOR_DISPLAYMANAGER_H
#define ORDINATOR_DISPLAYMANAGER_H

#include <TFT_eSPI.h>
#include "logo.h"

class DisplayManager
{
public:
    DisplayManager(HardwareSerial* hs, TFT_eSPI* tft);

    // Show the area41 logo
    void showBootLogo();

    void setFrameRate(uint8_t rate);
    bool nextFrame();
    bool everyXFrames(uint8_t frames);

    TFT_eSPI* getDisplay();

    uint8_t frameRate;
    uint16_t frameCount;
    uint8_t eachFrameMillis;
    long lastFrameStart;
    long nextFrameStart;
    bool post_render;
    uint8_t lastFrameDurationMs;

    void commenceSleep();

protected:

private:
    TFT_eSPI* _tft = nullptr;
    HardwareSerial* _hs = nullptr;
};

#endif //ORDINATOR_DISPLAYMANAGER_H
