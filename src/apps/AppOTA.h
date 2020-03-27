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
#ifndef ORDINATOR_APPOTA_H
#define ORDINATOR_APPOTA_H

#include <WiFiClient.h>
#include "../EventHandler.h"
#include "BaseApp.h"
#include "Update.h"
#include <WiFiManager.h>
#include <sstream>

class AppOTA : public BaseApp
{
public:
    AppOTA(HardwareSerial* hws, TFT_eSPI* s);
    ~AppOTA();

    void handleEvents(EventHandler* eh);
    void paintFrameInternal();

    bool canWeGoToSleep();

protected:


private:

    WiFiClient client;

    // Host => where your copy of Firmware Server runs at
    std::string host = "192.168.88.88";

    // Standard port of Firmware Server
    uint16_t port = 8081;

    // Standard URL of Firmware Server
    std::string bin = "/fw";

    const char* SSID = "hardac";
    const char* PWD = "h4rd4c.KUJH4D!";

    bool _isInUpdateMode = false;
    std::string _statusMessage = "";

    void execOTA();
    std::string getHeaderValue(std::string header, std::string headerName);
    void setStatusMessage(std::string message);
};

#endif //ORDINATOR_APPOTA_H
