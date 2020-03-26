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
#ifndef ORDINATOR_MODEABOUT_H
#define ORDINATOR_MODEABOUT_H

#include "../EventHandler.h"
#include "BaseMode.h"

class ModeAbout : public BaseMode
{
public:
    ModeAbout(HardwareSerial* hws, TFT_eSPI* s);
    ~ModeAbout();

    void handleEvents();
    void paintFrameInternal();

protected:
    uint8_t frameCount = 0;
    static const uint8_t everyXFrame = 7;

private:

    float_t d = 0.0;
    uint16_t position = 0; // position where in the string we are (left position of _tft)
    std::string scrolltext = "              "
                             "SWORDLORD - the coding crew "
                             "              "
                             " * LEGENDS NEVER DIE * "
                             "              "
                             "    * now with 100% more pinchability, nudge nudge *   ";
};

#endif // ORDINATOR_MODEABOUT_H
