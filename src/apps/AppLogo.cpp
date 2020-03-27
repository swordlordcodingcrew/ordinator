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
#include "AppLogo.h"

AppLogo::AppLogo(HardwareSerial* hws, TFT_eSPI* s) : BaseApp (hws, s)
{
    /* create a suitable fire palette, this is crucial for a good effect */
    /* black to blue, blue to red, red to yellow, yellow to white*/
/*
    int i;
    for (i = 0; i < 32; ++i)
    {
        /* black to blue, 32 values
        colours[i].b = i << 1;

        /* blue to red, 32 values
        colours[i + 32].r = i << 3;
        colours[i + 32].b =  64 - (i << 1);

        /*red to yellow, 32 values
        colours[i + 64].r = 255;
        colours[i + 64].g = i << 3;

        /* yellow to white, 162
        colours[i + 96].r = 255;
        colours[i + 96].g = 255;
        colours[i + 96].b = i << 2;
        colours[i + 128].r = 255;
        colours[i + 128].g = 255;
        colours[i + 128].b = 64 + (i << 2);
        colours[i + 160].r = 255;
        colours[i + 160].g = 255;
        colours[i + 160].b = 128 + (i << 2);
        colours[i + 192].r = 255;
        colours[i + 192].g = 255;
        colours[i + 192].b = 192 + i;
        colours[i + 224].r = 255;
        colours[i + 224].g = 255;
        colours[i + 224].b = 224 + i;
    }
*/
    //_offscreen->setPaletteColor()
    /*
     * uint16_t cmap[16];


  cmap[0]  = TFT_BLACK; // We will keep this as black
  cmap[1]  = TFT_NAVY;
  cmap[2]  = TFT_DARKGREEN;
  cmap[3]  = TFT_DARKCYAN;
  cmap[4]  = TFT_MAROON;
  cmap[5]  = TFT_PURPLE;
  cmap[6]  = TFT_PINK;
  cmap[7]  = TFT_LIGHTGREY;
  cmap[8]  = TFT_YELLOW;
  cmap[9]  = TFT_BLUE;
  cmap[10] = TFT_GREEN;
  cmap[11] = TFT_CYAN;
  cmap[12] = TFT_RED;
  cmap[13] = TFT_MAGENTA;
  cmap[14] = TFT_WHITE;  // Keep as white for text
  cmap[15] = TFT_BLUE;   // Keep as blue for sprite border

  // Pass the palette to the Sprite class
  spr.createPalette(cmap);
     */

}

AppLogo::~AppLogo() {}

bool AppLogo::canWeGoToSleep()
{
    return false;
}

void AppLogo::handleEvents()
{

}

void AppLogo::paintFrameInternal()
{
    _offscreen->pushImage(0, 0, 80, 160, shield);

    return;

    //_offscreen->fillSprite(TFT_BLACK);

    uint8_t x;
    uint8_t y;

    for (y = SCREEN_HEIGHT - 1; y > 100; y--)
    {
        for (x = 0; x < SCREEN_WIDTH; x++)
        {

            if(y == SCREEN_HEIGHT - 1)
            {
                int random = 1 + (int)(16.0 * (rand()/(RAND_MAX+1.0)));
                if (random > 9) {/* the lower the value, the intenser the fire, compensate a lower value with a higher decay value*/

                    _offscreen->drawPixel(x, y, TFT_RED);
                    //fire[y + x] = 255; /*maximum heat*/
                }
                else
                {
                    _offscreen->drawPixel(x, y, 0);
                    //fire[y + x] = 0;
                }
            }
            else
            {
                uint16_t temp;

                if (x == 0) /* at the left border*/
                {
                    temp = _offscreen->readPixel(x, y + 1);
                    temp += _offscreen->readPixel(x + 1, y + 1);
                    temp /= 2;

                    //temp = fire[y];
                    //temp += fire[y + 1];
                    //temp += fire[y - SCREEN_WIDTH];
                    //temp /=3;
                }
                else if (x == SCREEN_WIDTH - 1) /* at the right border*/
                {
                    temp = _offscreen->readPixel(x, y + 1);
                    temp += _offscreen->readPixel(x - 1, y + 1);
                    temp /= 2;

                    //temp = fire[y + x];
                    //temp += fire[y - SCREEN_WIDTH + x];
                    //temp += fire[y + x - 1];
                    //temp /= 3;
                }
                else
                {
                    temp = _offscreen->readPixel(x, y + 1);
                    temp += _offscreen->readPixel(x + 1, y + 1);
                    temp += _offscreen->readPixel(x - 1, y + 1);
                    temp >>= 2;
                    //temp = fire[y + x];
                    //temp += fire[y + x + 1];
                    //temp += fire[y + x - 1];
                    //temp += fire[y - SCREEN_WIDTH + x];
                    //temp >>= 2;
                }

                if (temp > 1)
                {
                    temp -= 1; /* decay */
                }

                _offscreen->drawPixel(x, y, temp);
                _hs->println("set: " + String(x) + "." + String(y) + "->" + String(temp));
                //fire[y - SCREEN_WIDTH + x] = temp;
            }
        }
    }

    _hs->println("loop done");
}




