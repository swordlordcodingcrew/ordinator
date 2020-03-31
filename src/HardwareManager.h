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
#include <WiFi.h>
#include <BLEDevice.h>
#include <rom/rtc.h>
#include <time.h>

class HardwareManager
{
public:
    HardwareManager(HardwareSerial* hws);

    void activateWifi();
    void deactivateWifi();
    bool isWifiRunning();
    bool isWifiConnected();
    void activateBLEServer();
    void deactivateBT();
    void deactivateBLEServer();
    bool isBTRunning();
    bool isBLEServerRunning();

    void commenceSleep();

    float getVoltage();
    uint8_t calcBatteryPercentage(float volts);
    bool isBatteryCharging();

    int16_t getBearing();
    void calibrateBearing();
    void calibrateGyro();
    float getTemperature();

    RTC_Date getCurrentTime();
    void adjustRTC();

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

    const char* ntpServer = "europe.pool.ntp.org";

    const float _BATTERY_MIN_V = 3.2;
    const float _BATTERY_MAX_V = 4.1;

    const std::string SERVICE_UUID = "5f4fc201-1fb5-459e-8fcc-c5c9c331914b";
    const std::string CHARACTERISTIC_UUID = "bab3483e-36e1-4688-b7f5-ea07361b26a8";

    const char* SSID = "hardac";
    const char* PWD = "h4rd4c.KUJH4D!";

    HardwareSerial* _hs = nullptr;
    MPU9250 _imu;
    PCF8563_Class _rtc;
    uint32_t _vref = 1100;
};

#endif //ORDINATOR_HARDWAREMANAGER_H
