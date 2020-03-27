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
#include "AppBattery.h"

AppBattery::AppBattery(HardwareSerial* hws, TFT_eSPI* s, HardwareManager* hwm) : BaseApp (hws, s), _hwm(hwm)
{

}

AppBattery::~AppBattery() {}

void AppBattery::handleEvents()
{

}

void AppBattery::paintFrameInternal()
{
    float voltage = _hwm->getVoltage();
    uint8_t percentage = _hwm->calcBatteryPercentage(voltage);

    char voltageString[5] = "";
    uint16_t originx = 3;
    uint16_t originy = 50;
    uint16_t width = 73;
    uint16_t height = 20;
    uint16_t tabheight = 15;
    uint16_t tabwidth = 5;
    uint8_t margin = 2;
    uint16_t barHeight = height - 2 * margin;
    uint16_t barWidth = width - 2 * margin;
    sprintf(voltageString, "%2.2f", voltage);

    _offscreen->fillScreen(TFT_BLACK);

    if (percentage == 0)
    {
        _offscreen->fillRoundRect(originx, originy, width, height, 3, TFT_BLACK);
    }

    _offscreen->fillRoundRect(originx - tabwidth + 1, (height - tabheight) / 2 + originy, tabwidth, tabheight, 1, TFT_WHITE);
    _offscreen->drawRoundRect(originx, originy, width, height, 3, TFT_WHITE);

    _offscreen->setTextColor(TFT_WHITE);
    _offscreen->setTextDatum(MC_DATUM);

    if (_hwm->isBatteryCharging())
    {
        _offscreen->fillRect(originx + margin, originy + margin, barWidth * percentage / 100.0, barHeight, TFT_RED);
        _offscreen->setTextPadding(_offscreen->textWidth(" Charging ", 2));
        _offscreen->drawString(" Charging ", width / 2 + originx, height / 2 + originy, 2);
    }
    else
    {
        _offscreen->fillRect(originx + margin, originy + margin, barWidth * percentage / 100.0, barHeight, TFT_DARKGREEN);
        _offscreen->setTextPadding(_offscreen->textWidth(" 888% ", 2));
        _offscreen->drawString(String(percentage) + "%", width / 2 + originx, height / 2 + originy, 2);
    }

}




