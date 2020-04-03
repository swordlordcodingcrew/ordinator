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
#include "AppManager.h"

AppManager::AppManager(HardwareSerial* hs, DisplayManager* dm, EventHandler* eh, HardwareManager* hwm) : _hs(hs), _dm(dm), _eh(eh), _hwm(hwm)
{
    setMode(BaseApp::APP_DEFAULT, BaseApp::APP_DEFAULT, false);
    // TODO check button during boot
    // check if user presses the button during boot
    // if so, start in ModeSelectMode::APP_DEFAULT
    // if not, init the mode to the last one from the config
    /*
    if(_eh->isLeftAndRightPressed())
    {
        _hs->println("reset into mode default");
        setMode(ModeSelectMode::APP_DEFAULT, ModeSelectMode::APP_DEFAULT, false);
    }
    else if(_eh->isPrgPressed())
    {
        _hs->println("you found the hidden secret. or so.");
        setMode(ModeSelectMode::APPID_ABOUT, ModeSelectMode::APP_DEFAULT, false);
    }
    else
    {
        _hs->println("booting into last selected mode");
        setMode(c->getCurrentAppID(), ModeSelectMode::APP_DEFAULT, false);
    }
    */
}

void AppManager::handleLoop()
{
    BaseApp* m = getCurrentApp();

    //_hs->println("handle events");
    m->handleEvents(_eh);

    //_hs->println("paint frame");
    m->paintFrame();
}


bool AppManager::appWantsEnforcedFramerate()
{
    BaseApp* m = getCurrentApp();

    if(m != nullptr)
    {
        return m->getEnforceFramerate();
    }
    else
    {
        return true;
    }
}

bool AppManager::canWeGoToSleep()
{
    BaseApp* m = getCurrentApp();

    if(m != nullptr)
    {
        return m->canWeGoToSleep();
    }
    else
    {
        return true;
    }
}

void AppManager::checkEvents()
{
    //_hs->println("checkEvents");

    /*

    // only allow cheat codes in specific apps
    if(_currentAppID == ModeSelectMode::APPID_LOGO || _currentAppID == ModeSelectMode::M_NICKNAME) {

        // check for easter eggs first
        std::string sKeyStream = _eh->getKeyStream();
        if (sKeyStream == "LRLRLLRR") {
            // make sure to not just trigger again
            _eh->clearKeyStream();

            uint8_t oldMode = _currentAppID;
            _currentAppID = ModeSelectMode::APPID_HELIX;
            setMode(_currentAppID, oldMode);
            return;
        } else if (sKeyStream == "RLRLRRLL") {
            // make sure to not just trigger again
            _eh->clearKeyStream();

            uint8_t oldMode = _currentAppID;
            _currentAppID = ModeSelectMode::APPID_HELIX;
            setMode(_currentAppID, oldMode);
            return;
        } else if (sKeyStream == "RLRRLRLL") {
            // make sure to not just trigger again
            _eh->clearKeyStream();

            uint8_t oldMode = _currentAppID;
            _currentAppID = ModeSelectMode::M_PLAY_TRACK;
            setMode(_currentAppID, oldMode);
            return;
        } else if (sKeyStream == "LRLLRLLR") {
            // make sure to not just trigger again
            _eh->clearKeyStream();

            uint8_t oldMode = _currentAppID;
            _currentAppID = ModeSelectMode::M_TELL_TALE;
            setMode(_currentAppID, oldMode);
            return;
        } else if (sKeyStream == "LLLLLLLR") {
            // make sure to not just trigger again
            _eh->clearKeyStream();

            uint8_t oldMode = _currentAppID;
            _currentAppID = ModeSelectMode::APPID_OTA;
            setMode(_currentAppID, oldMode);
            return;
        }
    }
     */

    // TODO
    // either we are in the long pressed config mode
    //  then we let the module do the configuration with the button presses
    // or we are in the normal mode
    //  then we switch to the next module
    if(_eh->isButtonJustReleased() && !_eh->buttonWasDownForAtLeast(2) && _currentApp->canSwitchAway())
    {
        log_d("Can switch to next mode");
        switchToNextMode();
    }
}

void AppManager::switchToNextMode()
{
    uint8_t newMode = _currentAppID + 1;

    if(newMode >= BaseApp::APP_COUNT)
    {
        newMode = 0;
    }

    setMode(newMode, _currentAppID, true);
}

uint8_t AppManager::getCurrentAppID()
{
    return _currentAppID;
}

BaseApp* AppManager::getCurrentApp()
{
    return _currentApp;
}

void AppManager::setMode(uint8_t newMode, uint8_t oldMode, bool storeMode)
{
    // mode changes, reset easter egg buffer
    //_eh->clearKeyStream();

    // remove the check for >= since easter egg apps are between 20 and XX
    // if(newMode < 0 || newMode >= ModeSelectMode::APP_COUNT)
    if(newMode < 0)
    {
        _currentAppID = BaseApp::APP_DEFAULT;
    }
    else
    {
        _currentAppID = newMode;
    }

    if(_currentApp)
    {
        //_hs->println("Freeing old object");

        _currentApp->cleanup();

        delete _currentApp;
    }

    log_i("New mode is: %d", _currentAppID);

    // make sure that the config file contains the current mode
    // stores only standard apps, no easter eggs...
    if(storeMode && _currentAppID < BaseApp::APP_COUNT)
    {
        // TODO fixme
        //conf->setCurrentMode(_currentAppID);
        //conf->storeConfig();
    }

    switch(_currentAppID)
    {
        case BaseApp::APPID_CLOCK_DIGITAL:
            _currentApp = new AppClockDigital(_hs, _dm->getDisplay(), _hwm);
            break;
        case BaseApp::APPID_CLOCK_KAERESTE:
            _currentApp = new AppClockKaereste(_hs, _dm->getDisplay(), _hwm);
            break;
        case BaseApp::APPID_LOGO:
            _currentApp = new AppLogo(_hs, _dm->getDisplay());
            break;
        case BaseApp::APPID_HELIX:
            _currentApp = new BaseApp(_hs, _dm->getDisplay());
            break;
        case BaseApp::APPID_ABOUT:
            _currentApp = new AppAbout(_hs, _dm->getDisplay());
            break;
        case BaseApp::APPID_BEARING:
            _currentApp = new AppBearing(_hs, _dm->getDisplay(), _hwm);
            break;
        case BaseApp::APPID_BATTERY:
            _currentApp = new AppBattery(_hs, _dm->getDisplay(), _hwm);
            break;
        case BaseApp::APPID_TEMPERATURE:
            _currentApp = new AppTemperature(_hs, _dm->getDisplay(), _hwm);
            break;
        case BaseApp::APPID_OTA:
            _currentApp = new AppOTA(_hs, _dm->getDisplay());
            break;
        default:
            // Just in case we f*** up, let's display the clock as default...
            _currentApp = new AppClockKaereste(_hs, _dm->getDisplay(), _hwm);
            break;
    }

    // make sure that the required hw is initialised
    _hwm->switchToRunLevel(_currentApp->runLevelNeeded());

    log_d("New App started");
}
