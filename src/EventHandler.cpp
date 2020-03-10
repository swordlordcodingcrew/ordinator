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
#include "EventHandler.h"

EventHandler::EventHandler(HardwareSerial* hws, EasyButton* btn) : hs(hws), _btn(btn)
{
    pinMode(TP_PWR_PIN, PULLUP);
    digitalWrite(TP_PWR_PIN, HIGH);
    pinMode(TP_PIN_PIN, INPUT_PULLUP);
}

void EventHandler::handleLoop()
{
    /*
    //tp_button.update();
    gpio_num_t pin = (gpio_num_t)(TP_PIN_PIN & 0x1F);
    int state=0;
    if (GPIO_REG_READ(GPIO_ENABLE1_REG) & BIT(pin)){
        //pin is output - read the GPIO_OUT_REG register
        state = (GPIO_REG_READ(GPIO_OUT1_REG)  >> pin) & 1U;
        hs->println(GPIO_REG_READ(GPIO_OUT_REG));
        hs->println(GPIO_REG_READ(GPIO_OUT1_REG));
    }
    else
    {
        //pin is input - read the GPIO_IN_REG register
        state = (GPIO_REG_READ(GPIO_IN1_REG)  >> pin) & 1U;
        hs->println(GPIO_REG_READ(GPIO_IN_REG));
        hs->println(GPIO_REG_READ(GPIO_IN1_REG));
    }
     */
    hs->println("__--__--__");
    hs->println(digitalRead(TP_PIN_PIN)); // this
    hs->println("__");

    hs->println(GPIO_REG_READ(GPIO_OUT_REG));
    hs->println(GPIO_REG_READ(GPIO_OUT1_REG));
    hs->println(GPIO_REG_READ(GPIO_IN_REG));
    hs->println(GPIO_REG_READ(GPIO_IN1_REG)); // this

    hs->println("**");

    hs->println(gpio_get_level(GPIO_NUM_33)); // this
    hs->println("==");

  /*
    __--__--__
    1 -> pressed
    __
    249561120
    0
    115592875
    2
    **
    1
    ==
    __--__--__
    0 -> unpressed
    __
    249561120
    0
    115592875
    0
    **
    0
    ==
    */

}

void EventHandler::shortPress()
{

}

void EventHandler::longPress()
{

}

