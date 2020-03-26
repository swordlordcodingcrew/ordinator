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

ModeManager::ModeManager(HardwareSerial* hs, DisplayManager* dm, EventHandler* eh, HardwareManager* hwm) : _hs(hs), _dm(dm), _eh(eh), _hwm(hwm)
{
    setMode(BaseMode::M_MODE_DEFAULT, BaseMode::M_MODE_DEFAULT, false);
    // TODO check button during boot
    // check if user presses the button during boot
    // if so, start in ModeSelectMode::M_MODE_DEFAULT
    // if not, init the mode to the last one from the config
    /*
    if(_eh->isLeftAndRightPressed())
    {
        _hs->println("reset into mode default");
        setMode(ModeSelectMode::M_MODE_DEFAULT, ModeSelectMode::M_MODE_DEFAULT, false);
    }
    else if(_eh->isPrgPressed())
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

void ModeManager::handleLoop()
{
    BaseMode* m = getCurrentModeObject();

    //_hs->println("handle events");
    m->handleEvents(_eh);

    //_hs->println("paint frame");
    m->paintFrame();
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

bool ModeManager::canWeGoToSleep()
{
    BaseMode* m = getCurrentModeObject();

    if(m != nullptr)
    {
        return m->canWeGoToSleep();
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
    if(_currentMode == ModeSelectMode::M_LOGO || _currentMode == ModeSelectMode::M_NICKNAME) {

        // check for easter eggs first
        std::string sKeyStream = _eh->getKeyStream();
        if (sKeyStream == "LRLRLLRR") {
            // make sure to not just trigger again
            _eh->clearKeyStream();

            uint8_t oldMode = _currentMode;
            _currentMode = ModeSelectMode::M_HELIX;
            setMode(_currentMode, oldMode);
            return;
        } else if (sKeyStream == "RLRLRRLL") {
            // make sure to not just trigger again
            _eh->clearKeyStream();

            uint8_t oldMode = _currentMode;
            _currentMode = ModeSelectMode::M_HELIX;
            setMode(_currentMode, oldMode);
            return;
        } else if (sKeyStream == "RLRRLRLL") {
            // make sure to not just trigger again
            _eh->clearKeyStream();

            uint8_t oldMode = _currentMode;
            _currentMode = ModeSelectMode::M_PLAY_TRACK;
            setMode(_currentMode, oldMode);
            return;
        } else if (sKeyStream == "LRLLRLLR") {
            // make sure to not just trigger again
            _eh->clearKeyStream();

            uint8_t oldMode = _currentMode;
            _currentMode = ModeSelectMode::M_TELL_TALE;
            setMode(_currentMode, oldMode);
            return;
        } else if (sKeyStream == "LLLLLLLR") {
            // make sure to not just trigger again
            _eh->clearKeyStream();

            uint8_t oldMode = _currentMode;
            _currentMode = ModeSelectMode::M_OTA;
            setMode(_currentMode, oldMode);
            return;
        }
    }
     */

    // TODO
    // either we are in the long pressed config mode
    //  then we let the module do the configuration with the button presses
    // or we are in the normal mode
    //  then we switch to the next module

    if(_eh->isButtonJustReleased() && !_eh->buttonWasDownForAtLeast(2) && _currentModeObject->canSwitchAway())
    {
        switchToNextMode();
    }
}

void ModeManager::switchToNextMode()
{
    uint8_t newMode = _currentMode + 1;

    if(newMode >= BaseMode::M_MODE_COUNT)
    {
        newMode = 0;
    }

    setMode(newMode, _currentMode, true);
}

uint8_t ModeManager::getCurrentMode()
{
    return _currentMode;
}

BaseMode* ModeManager::getCurrentModeObject()
{
    return _currentModeObject;
}

void ModeManager::setMode(uint8_t newMode, uint8_t oldMode, bool storeMode)
{
    // mode changes, reset easter egg buffer
    //_eh->clearKeyStream();

    // remove the check for >= since easter egg modes are between 20 and XX
    // if(newMode < 0 || newMode >= ModeSelectMode::M_MODE_COUNT)
    if(newMode < 0)
    {
        _currentMode = BaseMode::M_MODE_DEFAULT;
    }
    else
    {
        _currentMode = newMode;
    }

    if(_currentModeObject)
    {
        //_hs->println("Freeing old object");

        _currentModeObject->cleanup();

        delete _currentModeObject;
    }

    _hs->print("New mode is: ");
    _hs->println(_currentMode);

    // make sure that the config file contains the current mode
    // stores only standard modes, no easter eggs...
    if(storeMode && _currentMode < BaseMode::M_MODE_COUNT)
    {
        // TODO fixme
        //conf->setCurrentMode(_currentMode);
        //conf->storeConfig();
    }

    //    static const uint8_t M_CLOCK        = 1;
    //    static const uint8_t M_LOGO         = 2;
    //    static const uint8_t M_HELIX        = 3;
    //    static const uint8_t M_ABOUT        = 4;
    //    static const uint8_t M_BEARING      = 5;
    //    static const uint8_t M_OTA          = 6;
    switch(_currentMode)
    {
        case BaseMode::M_CLOCK:
            _currentModeObject = new ModeClock(_hs, _dm->getDisplay(), _hwm);
            break;
        case BaseMode::M_LOGO:
            _currentModeObject = new ModeLogo(_hs, _dm->getDisplay());
            break;
        case BaseMode::M_HELIX:
            _currentModeObject = new BaseMode(_hs, _dm->getDisplay());
            break;
        case BaseMode::M_ABOUT:
            _currentModeObject = new ModeAbout(_hs, _dm->getDisplay());
            break;
        case BaseMode::M_BEARING:
            _currentModeObject = new ModeBearing(_hs, _dm->getDisplay(), _hwm);
            break;
        case BaseMode::M_OTA:
            _currentModeObject = new ModeOTA(_hs, _dm->getDisplay());
            break;
        default:
            // Just in case we f*** up, let's display the clock as default...
            _currentModeObject = new ModeClock(_hs, _dm->getDisplay(), _hwm);
            break;
    }

    _hs->println("New mode started");
}
