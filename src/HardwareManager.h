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
#ifndef ORDINATOR_HARDWAREMANAGER_H
#define ORDINATOR_HARDWAREMANAGER_H

#include <HardwareSerial.h>
#include "hal.hpp"
#include <EEPROM.h>
#include <esp_adc_cal.h>
#include <pcf8563.h>
#include <MPU9250.hpp>
#include "FS.h"
#include "SPIFFS.h"
#include <WiFiUdp.h>
#include <NTP.h>
#include <WiFi.h>

class HardwareManager
{
public:
    HardwareManager(HardwareSerial* hws);

    void activateWifi();
    void deactivateWifi();
    void btStop();
    float getVoltage();
    uint8_t calcBatteryPercentage(float volts);

    int16_t getBearing();
    void calibrateBearing();
    void calibrateGyro();
    float getTemperature();

    void commenceSleep();

    RTC_Date getCurrentTime();
    void adjustRTC();

    // TODO thinking about having a loop method and putting that in there...
    void updateChargeLED();

protected:

    const int _MAG_CALIBRATION_ADDRESS = 0x00;

    void storeMagBiasInEEPROM(float* magbias);
    void getMagBiasFromEEPROM(float* magbias);

    void setupADC();
    void setupBattery();
    void initMPU();
    void mpuSleep();

    void initClock();
    void rtcSleep();

    void initSPIFFS();

private:

    const float _BATTERY_MIN_V = 3.2;
    const float _BATTERY_MAX_V = 4.1;

    const char* SSID = "hardac";
    const char* PWD = "h4rd4c.KUJH4D!";

    bool isCharging();

    HardwareSerial* _hs = nullptr;
    MPU9250 _imu;
    PCF8563_Class _rtc;
    uint32_t _vref = 1100;
};

#endif //ORDINATOR_HARDWAREMANAGER_H
