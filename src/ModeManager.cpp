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
#include "ModeManager.h"

ModeManager::ModeManager(HardwareSerial* hs, TFT_eSPI* tft) : _hs(hs), _tft(tft)
{
    setMode(BaseMode::M_MODE_DEFAULT, BaseMode::M_MODE_DEFAULT, false);
    // TODO check button during boot
    // check if user presses the button during boot
    // if so, start in ModeSelectMode::M_MODE_DEFAULT
    // if not, init the mode to the last one from the config
    /*
    if(eh->isLeftAndRightPressed())
    {
        _hs->println("reset into mode default");
        setMode(ModeSelectMode::M_MODE_DEFAULT, ModeSelectMode::M_MODE_DEFAULT, false);
    }
    else if(eh->isPrgPressed())
    {
        _hs->println("you found the hidden secret. or so.");
        setMode(ModeSelectMode::M_ABOUT, ModeSelectMode::M_MODE_DEFAULT, false);
    }
    else
    {
        _hs->println("booting into last selected mode");
        setMode(c->getCurrentMode(), ModeSelectMode::M_MODE_DEFAULT, false);
    }
    */
}

bool ModeManager::moduleWantsEnforcedFramerate()
{
    BaseMode* m = getCurrentModeObject();

    if(m != nullptr)
    {
        return m->getEnforceFramerate();
    }
    else
    {
        return true;
    }
}

void ModeManager::checkEvents()
{
    //_hs->println("checkEvents");

    /*

    // only allow cheat codes in specific modes
    if(currentMode == ModeSelectMode::M_LOGO || currentMode == ModeSelectMode::M_NICKNAME) {

        // check for easter eggs first
        std::string sKeyStream = eh->getKeyStream();
        if (sKeyStream == "LRLRLLRR") {
            // make sure to not just trigger again
            eh->clearKeyStream();

            uint8_t oldMode = currentMode;
            currentMode = ModeSelectMode::M_HELIX;
            setMode(currentMode, oldMode);
            return;
        } else if (sKeyStream == "RLRLRRLL") {
            // make sure to not just trigger again
            eh->clearKeyStream();

            uint8_t oldMode = currentMode;
            currentMode = ModeSelectMode::M_HELIX;
            setMode(currentMode, oldMode);
            return;
        } else if (sKeyStream == "RLRRLRLL") {
            // make sure to not just trigger again
            eh->clearKeyStream();

            uint8_t oldMode = currentMode;
            currentMode = ModeSelectMode::M_PLAY_TRACK;
            setMode(currentMode, oldMode);
            return;
        } else if (sKeyStream == "LRLLRLLR") {
            // make sure to not just trigger again
            eh->clearKeyStream();

            uint8_t oldMode = currentMode;
            currentMode = ModeSelectMode::M_TELL_TALE;
            setMode(currentMode, oldMode);
            return;
        } else if (sKeyStream == "LLLLLLLR") {
            // make sure to not just trigger again
            eh->clearKeyStream();

            uint8_t oldMode = currentMode;
            currentMode = ModeSelectMode::M_OTA;
            setMode(currentMode, oldMode);
            return;
        }
    }

    if(eh->isPrgJustPressed())
    {
        // check if just left MODE_SELECTMODE
        // if so, set new mode to selected mode
        // if not change to select mode

        if(currentMode == ModeSelectMode::M_SELECT_MODE)
        {
            ModeSelectMode* m = (ModeSelectMode*)getCurrentModeObject();

            _hs->print("Selected Mode is: ");
            _hs->println(m->getSelectedMode());

            currentMode = m->getSelectedMode();
            setMode(currentMode, 0);
        }
        else
        {
            uint8_t oldMode = currentMode;
            currentMode = 0;
            setMode(currentMode, oldMode);
        }
    }
     */
}

uint8_t ModeManager::getCurrentMode()
{
    return currentMode;
}

BaseMode* ModeManager::getCurrentModeObject()
{
    return currentModeObject;
}

void ModeManager::setMode(uint8_t newMode, uint8_t oldMode, bool storeMode)
{
    // mode changes, reset easter egg buffer
    //eh->clearKeyStream();

    // remove the check for >= since easter egg modes are between 20 and XX
    // if(newMode < 0 || newMode >= ModeSelectMode::M_MODE_COUNT)
    if(newMode < 0)
    {
        currentMode = BaseMode::M_MODE_DEFAULT;
    }
    else
    {
        currentMode = newMode;
    }

    if(currentModeObject)
    {
        //_hs->println("Freeing old object");

        currentModeObject->cleanup();

        delete currentModeObject;
    }

    _hs->print("New mode is: ");
    _hs->println(currentMode);

    // make sure that the config file contains the current mode
    // stores only standard modes, no easter eggs...
    if(storeMode && currentMode < BaseMode::M_MODE_COUNT)
    {
        // TODO fixme
        //conf->setCurrentMode(currentMode);
        //conf->storeConfig();
    }

    switch(currentMode)
    {
        case BaseMode::M_HELIX:
            currentModeObject = new BaseMode(_hs, _tft);
            break;

        case BaseMode::M_ABOUT:
            currentModeObject = new ModeAbout(_hs, _tft);
            break;
        default:
            currentModeObject = new BaseMode(_hs, _tft);
            break;
    }

    _hs->println("Object instantiated");
}
