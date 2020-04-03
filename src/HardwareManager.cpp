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

#include "HardwareManager.h"

HardwareManager::HardwareManager(HardwareSerial* hws) : _hs(hws)
{
    log_d("Initialising Hardware Manager");
    initRL1();
}

RunLevel HardwareManager::getRunLevel()
{
    return _runLevel;
}

void HardwareManager::initRL1()
{
    if(_runLevel != RL0)
    {
        log_w("Can't initialise RunLevel 1 (%d)", _runLevel);
        return;
    }

    log_i("Moving up to RunLevel 1");
    setCpuFrequencyMhz(80);

    log_i("CPU speed is %d", getCpuFrequencyMhz());
    log_i("CPU reset codes. CPU0: %d %s, CPU1: %d %s",
            rtc_get_reset_reason(0), translateResetReason(rtc_get_reset_reason(0)),
            rtc_get_reset_reason(1), translateResetReason(rtc_get_reset_reason(1)));

    initClock();
    setupADC();
    setupBattery();

    _runLevel = RL1;
}

void HardwareManager::initRL2()
{
    if(_runLevel != RL1)
    {
        log_w("Can't initialise RunLevel 2 (%d)", _runLevel);
        return;
    }

    log_i("Moving up to RunLevel 2");

    initSPIFFS();

    _runLevel = RL2;
}

void HardwareManager::terminateRL2()
{
    if(_runLevel != RL2)
    {
        log_w("Can't terminate RunLevel 2 (%d)", _runLevel);
        return;
    }

    SPIFFS.end();

    log_i("Terminating RunLevel 2, switching to RunLevel 1");
    _runLevel = RL1;
}

void HardwareManager::initRL3()
{
    if(_runLevel != RL2)
    {
        log_w("Can't initialise RunLevel 3 (%d)", _runLevel);
        return;
    }

    log_i("Moving up to RunLevel 3");
    // todo we could think about quicker clock speed in this mode...
    initMPU();

    _runLevel = RL3;
}

void HardwareManager::terminateRL3()
{
    if(_runLevel != RL3)
    {
        log_w("Can't terminate RunLevel 3 (%d)", _runLevel);
        return;
    }

    log_i("Terminating RunLevel 3, switching to RunLevel 2");
    mpuSleep();
    deactivateWifi();
    deactivateBT();

    _runLevel = RL2;
}

void HardwareManager::commenceSleep()
{
    _runLevel = RL0;
    log_i("Going into sleep mode");
    delay(20);

    rtcSleep();
    pinMode(39, GPIO_MODE_INPUT);
    esp_sleep_enable_ext1_wakeup(GPIO_SEL_33 | GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_deep_sleep_disable_rom_logging();
    esp_deep_sleep_start();
}

bool HardwareManager::switchToRunLevel(RunLevel requestedRL)
{
    while(requestedRL != _runLevel)
    {
        if(requestedRL > _runLevel)
        {
            // moving up
            switch(_runLevel)
            {
                case RL1:
                    initRL2();
                    break;
                case RL2:
                    initRL3();
                    break;
                default:
                    log_e("non-reachable runlevel status reached, device will go up in flames now.");
                    return false;
            }
        }
        else
        {
            // going down
            switch(_runLevel)
            {
                case RL3:
                    terminateRL3();
                    break;
                case RL2:
                    terminateRL2();
                    break;
                case RL1:
                    commenceSleep();
                    break;
                default:
                    log_e("non-reachable runlevel status reached, device will go up in flames now.");
                    return false;
            }
        }
    }

    return true;
}

void HardwareManager::updateChargeLED()
{
    digitalWrite(LED_PIN, isBatteryCharging());
}

bool HardwareManager::isBatteryCharging()
{
    return !digitalRead(CHARGE_PIN);
}

void HardwareManager::initSPIFFS()
{
    // format spiffs if failed
    if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS Mount Failed");
    }
}

void HardwareManager::initClock()
{
    _rtc.begin(Wire);
    //_rtc.check();
}

void HardwareManager::rtcSleep()
{
    _rtc.clearTimer();
    _rtc.resetAlarm();
    _rtc.disableAlarm();
    _rtc.disableCLK();
    _rtc.disableTimer();
}

RTC_Date HardwareManager::getCurrentTime()
{
    return _rtc.getDateTime();
}

void HardwareManager::adjustRTC()
{
    activateWifi();

//    ntp.ruleDST("CEST", Last, Sun, Mar, 2, 120); // last sunday in march 2:00, timetone +120min (+1 GMT + 1h summertime offset)
//    ntp.ruleSTD("CET", Last, Sun, Oct, 3, 60); // last sunday in october 3:00, timezone +60min (+1 GMT)

    const long  gmtOffset_sec = 3600;
    const int   daylightOffset_sec = 3600;

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    struct tm t;
    if(!getLocalTime(&t))
    {
        _hs->println("Failed to obtain time, not updating");
    }
    else
    {
        _hs->println(&t, "New date/time: %A, %B %d %Y %H:%M:%S");

        // year is since 1900
        // month is starting at 0
        RTC_Date datetime = RTC_Date(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
        _hs->print("Setting time to: ");
        _hs->print(datetime.hour);
        _hs->print(":");
        _hs->print(datetime.minute);
        _hs->print(":");
        _hs->print(datetime.second);
        _hs->print(" at the ");
        _hs->print(datetime.day);
        _hs->print(".");
        _hs->print(datetime.month);
        _hs->print(".");
        _hs->println(datetime.year);

        _rtc.setDateTime(datetime);
    }

    deactivateWifi();
}

void HardwareManager::activateWifi()
{
    WiFi.mode(WIFI_STA);
    delay(1);
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(SSID, PWD);

        uint64_t now = millis();
        while(now + 5000 < millis() || WiFi.status() != WL_CONNECTED)
        {
            delay(10);
            _hs->print(".");
        }
        _hs->println(WiFi.status() == WL_CONNECTED);
    }
}
void HardwareManager::deactivateWifi()
{
    WiFi.mode(WIFI_OFF);
}

bool HardwareManager::isWifiRunning()
{

}

bool HardwareManager::isWifiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void HardwareManager::activateBLEServer()
{
    BLEDevice::init(std::string("Ordinator.") + ESP.getSketchMD5().c_str());
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setValue("Hello World says the Ordinator");
    pService->start();
    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

void HardwareManager::deactivateBT()
{
    btStop();
}

void HardwareManager::deactivateBLEServer()
{
    btStop();
}

bool HardwareManager::isBTRunning()
{
    return btStarted();
}

bool HardwareManager::isBLEServerRunning()
{
    return btStarted();
}

void HardwareManager::setupBattery()
{
    pinMode(CHARGE_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

void HardwareManager::setupADC()
{
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
            (adc_unit_t)ADC_UNIT_1,
            (adc_atten_t)ADC1_CHANNEL_6,
            (adc_bits_width_t)ADC_WIDTH_BIT_12,
            1100,
            &adc_chars);
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        _vref = adc_chars.vref;
    }
}

float HardwareManager::getVoltage()
{
    uint16_t v = analogRead(BATT_ADC_PIN);

    // Todo fixme
    float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (_vref / 1000.0);

    return battery_voltage;
}

uint8_t HardwareManager::calcBatteryPercentage(float volts)
{
    // Todo fixme
    float percentage = (volts - _BATTERY_MIN_V) * 100 / (_BATTERY_MAX_V - _BATTERY_MIN_V);
    if (percentage > 100)
    {
        percentage = 100;
    }
    if (percentage < 0)
    {
        percentage = 0;
    }
    return percentage;
}

void HardwareManager::initMPU()
{
    uint8_t c = _imu.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    if (c == 0x71)
    {
        _imu.initMPU9250();
        _imu.initAK8963(_imu.magCalibration);
        getMagBiasFromEEPROM(_imu.magbias);
    }
}

void HardwareManager::mpuSleep()
{
    _imu.writeBit(MPU9250_ADDRESS, PWR_MGMT_1, 5, false);
    _imu.writeBit(MPU9250_ADDRESS, PWR_MGMT_1, 6, false);
    _imu.writeBit(MPU9250_ADDRESS, PWR_MGMT_1, 4, false);
    _imu.writeByte(MPU9250_ADDRESS, PWR_MGMT_2, 0x07);
    _imu.writeByte(MPU9250_ADDRESS, ACCEL_CONFIG2, 0x09);
    _imu.writeByte(MPU9250_ADDRESS, INT_ENABLE, 0x40);
    _imu.writeBit(MPU9250_ADDRESS, MOT_DETECT_CTRL, 7, true);
    _imu.writeBit(MPU9250_ADDRESS, MOT_DETECT_CTRL, 6, true);
    _imu.writeByte(MPU9250_ADDRESS, WOM_THR, 220);
    _imu.writeBit(MPU9250_ADDRESS, INT_PIN_CFG, 5, 0);
    _imu.writeByte(MPU9250_ADDRESS, INT_STATUS, 0);
    _imu.writeByte(MPU9250_ADDRESS, LP_ACCEL_ODR, 0x01);
    _imu.writeBit(MPU9250_ADDRESS, PWR_MGMT_1, 5, true);
}

int16_t HardwareManager::getBearing()
{
    _imu.readMagData(_imu.magCount); // Read the x/y/z adc values
    _imu.getMres();
    _imu.mx = (float)_imu.magCount[0] * _imu.mRes * _imu.magCalibration[0] - _imu.magbias[0];
    _imu.my = (float)_imu.magCount[1] * _imu.mRes * _imu.magCalibration[1] - _imu.magbias[1];
    _imu.mz = (float)_imu.magCount[2] * _imu.mRes * _imu.magCalibration[2] - _imu.magbias[2];

    float bearing = atan2(_imu.my, _imu.mx);

    return (bearing > 0 ? bearing : (2 * M_PI + bearing)) * 360 / (2 * M_PI);

}

void HardwareManager::calibrateBearing()
{
    uint16_t i = 0;
    uint16_t sample_count = 1500;

    int32_t mag_bias[3] = {0, 0, 0};
    int16_t mag_max[3] = {-32767, -32767, -32767};
    int16_t mag_min[3] = {32767, 32767, 32767};
    int16_t mag_temp[3] = {0, 0, 0};

    delay(4);

    for (i = 0; i < sample_count; i++)
    {
        _imu.readMagData(mag_temp); // Read the mag data
        for (int jj = 0; jj < 3; jj++)
        {
            if (mag_temp[jj] > mag_max[jj])
            {
                mag_max[jj] = mag_temp[jj];
            }
            if (mag_temp[jj] < mag_min[jj])
            {
                mag_min[jj] = mag_temp[jj];
            }
        }

        delay(12);                                                              // at 100 Hz ODR, new mag data is available every 10 ms

        mag_bias[0] = (mag_max[0] + mag_min[0]) / 2;                            // get average x mag bias in counts
        mag_bias[1] = (mag_max[1] + mag_min[1]) / 2;                            // get average y mag bias in counts
        mag_bias[2] = (mag_max[2] + mag_min[2]) / 2;                            // get average z mag bias in counts

        _imu.magbias[0] = (float)mag_bias[0] * _imu.mRes * _imu.magCalibration[0]; // save mag biases in G for main program
        _imu.magbias[1] = (float)mag_bias[1] * _imu.mRes * _imu.magCalibration[1];
        _imu.magbias[2] = (float)mag_bias[2] * _imu.mRes * _imu.magCalibration[2];
    }

    storeMagBiasInEEPROM(_imu.magbias);
}

float HardwareManager::getTemperature()
{
    int16_t tempCount = _imu.readTempData();
    float temperature = ((float)tempCount) / 333.87 + 21.0;
    return temperature;
}

void HardwareManager::storeMagBiasInEEPROM(float *magbias)
{
    // fixme
    return;

    EEPROM.begin(3 * sizeof(float));
    for (uint8_t i = 0; i < 3; i++)
    {
        EEPROM.put(_MAG_CALIBRATION_ADDRESS + (i * sizeof(float)), magbias[i]);
    }
    EEPROM.commit();
    EEPROM.end();
}

void HardwareManager::getMagBiasFromEEPROM(float *magbias)
{
    // fixme
    return;

    EEPROM.begin(3 * sizeof(float));
    for (uint8_t i = 0; i < 3; i++)
    {
        magbias[i] = EEPROM.read(_MAG_CALIBRATION_ADDRESS + (i * sizeof(float)));
    }
    EEPROM.end();
}

std::string translateResetReason(RESET_REASON reason)
{
    switch ( reason)
    {
        case 1 : return "POWERON_RESET";            /**<1, Vbat power on reset*/
        case 3 : return "SW_RESET";                 /**<3, Software reset digital core*/
        case 4 : return "OWDT_RESET";               /**<4, Legacy watch dog reset digital core*/
        case 5 : return "DEEPSLEEP_RESET";          /**<5, Deep Sleep reset digital core*/
        case 6 : return "SDIO_RESET";               /**<6, Reset by SLC module, reset digital core*/
        case 7 : return "TG0WDT_SYS_RESET";         /**<7, Timer Group0 Watch dog reset digital core*/
        case 8 : return "TG1WDT_SYS_RESET";         /**<8, Timer Group1 Watch dog reset digital core*/
        case 9 : return "RTCWDT_SYS_RESET";         /**<9, RTC Watch dog Reset digital core*/
        case 10 : return "INTRUSION_RESET";         /**<10, Intrusion tested to reset CPU*/
        case 11 : return "TGWDT_CPU_RESET";         /**<11, Time Group reset CPU*/
        case 12 : return "SW_CPU_RESET";            /**<12, Software reset CPU*/
        case 13 : return "RTCWDT_CPU_RESET";        /**<13, RTC Watch dog Reset CPU*/
        case 14 : return "EXT_CPU_RESET";           /**<14, for APP CPU, reset by PRO CPU*/
        case 15 : return "RTCWDT_BROWN_OUT_RESET";  /**<15, Reset when the vdd voltage is not stable*/
        case 16 : return "RTCWDT_RTC_RESET";        /**<16, RTC Watch dog reset digital core and rtc module*/
        default : return "UNKNOWN_REASON";
    }
}