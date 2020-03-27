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
#ifndef ORDINATOR_APPLOGO_H
#define ORDINATOR_APPLOGO_H

#include "../EventHandler.h"
#include "BaseApp.h"
#include "logo.h"

/*
typedef struct{
    byte r;
    byte g;
    byte b;
    byte unused;
} SL_Colour;
 */

class AppLogo : public BaseApp
{
public:
    AppLogo(HardwareSerial* hws, TFT_eSPI* s);
    ~AppLogo();

    void handleEvents();
    void paintFrameInternal();

    bool canWeGoToSleep();

protected:

private:

    static const uint16_t SCREEN_WIDTH = 80;
    static const uint16_t SCREEN_HEIGHT = 160;

    //static SL_Colour colours[256];
    //uint8_t fire[SCREEN_WIDTH * SCREEN_HEIGHT];
};

#endif //ORDINATOR_APPLOGO_H
