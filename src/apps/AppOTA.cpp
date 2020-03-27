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
#include "AppOTA.h"

AppOTA::AppOTA(HardwareSerial* hws, TFT_eSPI* s) : BaseApp (hws, s)
{
    _tft->fillScreen(TFT_BLUE);
}

AppOTA::~AppOTA()
{
    WiFi.mode(WIFI_OFF);
}

bool AppOTA::canWeGoToSleep()
{
    return false;
}

void AppOTA::handleEvents(EventHandler* eh)
{
    if(eh->isButtonJustReleased() && eh->buttonWasDownForAtLeast(2))
    {
        // TODO fixme, have the hardwaremanager do this
        // Connect to provided SSID and PSWD
        WiFi.begin(SSID, PWD);
        _isInUpdateMode = true;
        setStatusMessage("FW Update");
    }

    if(_isInUpdateMode)
    {
        // Wait for connection to establish
        if (WiFi.status() == WL_CONNECTED) {
            _isInUpdateMode = false;
            execOTA();
        }
    }
}

void AppOTA::paintFrameInternal()
{
    _offscreen->fillScreen(TFT_BLUE);
    _offscreen->setTextColor(TFT_BLACK);
    _offscreen->setTextDatum(MC_DATUM);
    _offscreen->drawString(_statusMessage.c_str(), _tft->width() / 2, _tft->height() / 2);
}

// Utility to extract header value from headers
std::string AppOTA::getHeaderValue(std::string header, std::string headerName)
{
    return header.substr(strlen(headerName.c_str()));
}

void AppOTA::setStatusMessage(std::string message)
{
    _statusMessage = message;
    _hs->print("OTA: ");
    _hs->println(message.c_str());
}

// OTA Logic, with a little help from the espressif samples at
// https://github.com/espressif/arduino-esp32/blob/master/libraries/Update/examples/
void AppOTA::execOTA()
{
    // Variables to validate response from Firmware Server
    long contentLength = 0;
    bool isValidContentType = false;

    setStatusMessage(std::string("Connecting to: ") + host);

    // Connect to Firmware Server
    if (client.connect(host.c_str(), port))
    {
        // Connection Succeed.
        // Fecthing the bin
        setStatusMessage(std::string("Fetching Bin: ") + bin);

        // Get the contents of the bin file
        std::stringstream request;

        request << "GET " << bin << " HTTP/1.1\r\n" <<
                "Host: " + host + "\r\n" <<
                "Cache-Control: no-cache\r\n" <<
                "Connection: close\r\n\r\n";

        // Check what is being sent
        setStatusMessage(request.str());

        client.print(request.str().c_str());

        unsigned long timeout = millis();

        while (client.available() == 0)
        {
            if (millis() - timeout > 5000)
            {
                setStatusMessage("Client Timeout !");
                client.stop();
                return;
            }
        }
        // Once the response is available, check response

        /*
         Response should look like

            HTTP/1.1 200 OK
            Date: Wed, 14 Jun 2017 03:33:59 GMT
            Last-Modified: Fri, 02 Jun 2017 14:50:11 GMT
            Content-Type: application/octet-stream
            Content-Length: 357280

            {{BIN FILE CONTENTS}}
        */
        while (client.available())
        {
            // read line till /n
            String line = client.readStringUntil('\n');
            // remove space, to check if the line is end of headers
            line.trim();

            // if the the line is empty,
            // this is end of headers
            // break the while and feed the
            // remaining `client` to the
            // Update.writeStream();
            if (!line.length()) {
                //headers ended
                break; // and get the OTA started
            }

            // Check if the HTTP Response is 200
            // else break and Exit Update
            if (line.startsWith("HTTP/1.1")) {
                if (line.indexOf("200") < 0) {
                    _hs->println("Got a non 200 status code from server. Exiting OTA Update.");
                    setStatusMessage("Wrong status");
                    break;
                }
            }

            // extract headers here
            // Start with content length
            if (line.startsWith("Content-Length: "))
            {
                contentLength = atol((getHeaderValue(line.c_str(), "Content-Length: ")).c_str());
                _hs->println("Got " + String(contentLength) + " bytes from server");
            }

            // Next, the content type
            if (line.startsWith("Content-Type: "))
            {
                std::string contentType = getHeaderValue(line.c_str(), "Content-Type: ");
                _hs->print("Got ");
                _hs->print(contentType.c_str());
                _hs->println(" payload.");
                if (contentType == "application/octet-stream")
                {
                    isValidContentType = true;
                }
            }
        }
    }
    else
    {
        // Connect to Firmware Server failed
        _hs->println(("Connection to " + host + " failed. Please check your setup").c_str());
        setStatusMessage("Connection failed");
    }

    // Check what is the contentLength and if content type is `application/octet-stream`
    _hs->println("contentLength : " + String(contentLength) + ", isValidContentType : " + String(isValidContentType));

    // check contentLength and content type
    if (contentLength && isValidContentType)
    {
        // Check if there is enough to OTA Update
        bool canBegin = Update.begin(contentLength);

        // If yes, begin
        if (canBegin)
        {
            _hs->println("Begin OTA. This may take 2 - 5 mins to complete. Things might be quite for a while.. Patience!");
            setStatusMessage("Begin update");
            // No activity would appear on the Serial monitor
            // So be patient. This may take 2 - 5mins to complete
            size_t written = Update.writeStream(client);

            if (written == contentLength)
            {
                _hs->println("Written : " + String(written) + " successfully");
            }
            else
            {
                _hs->println("Written only : " + String(written) + "/" + String(contentLength) + ". Retry?" );
            }

            if (Update.end())
            {
                _hs->println("OTA done!");
                setStatusMessage("Success");

                if (Update.isFinished())
                {
                    _hs->println("Update successfully completed. Rebooting.");
                    ESP.restart();
                }
                else
                {
                    _hs->println("Update not finished? Something went wrong!");
                }
            }
            else
            {
                _hs->println("Error Occurred. Error #: " + String(Update.getError()));
            }
        }
        else
        {
            // not enough space to begin OTA
            // Understand the partitions and
            // space availability
            _hs->println("Not enough space to begin OTA");
            setStatusMessage("Not enough space");
            client.flush();
        }
    }
    else
    {
        _hs->println("There was no content in the response");
        setStatusMessage("No content");
        client.flush();
    }
}




