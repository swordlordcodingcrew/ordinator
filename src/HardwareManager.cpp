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
    setupADC();
    initMPU();
    initClock();
    setupBattery();
}

void HardwareManager::commenceSleep()
{
    mpuSleep();
    deactivateWifi();
    rtcSleep();
    pinMode(39, GPIO_MODE_INPUT);
    esp_sleep_enable_ext1_wakeup(GPIO_SEL_33 | GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_deep_sleep_disable_rom_logging();
    esp_deep_sleep_start();
}

void HardwareManager::updateChargeLED()
{
    digitalWrite(LED_PIN, isCharging());
}

bool HardwareManager::isCharging() {

    return !digitalRead(CHARGE_PIN);
}

void HardwareManager::initClock()
{
    _rtc.begin(Wire);
    _rtc.check();
}

void HardwareManager::rtcSleep()
{
    _rtc.clearTimer();
    _rtc.resetAlarm();
    _rtc.disableAlarm();
    _rtc.disableCLK();
    _rtc.disableTimer();
}

RTC_Date HardwareManager::getClockTime()
{
    return _rtc.getDateTime();
}

void HardwareManager::activateWifi(){}
void HardwareManager::deactivateWifi(){}
void HardwareManager::btStop(){}

void HardwareManager::setupBattery()
{
    pinMode(CHARGE_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

void HardwareManager::setupADC()
{
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize((adc_unit_t)ADC_UNIT_1, (adc_atten_t)ADC1_CHANNEL_6, (adc_bits_width_t)ADC_WIDTH_BIT_12, 1100, &adc_chars);
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        _vref = adc_chars.vref;
    }
}

float HardwareManager::getVoltage()
{
    uint16_t v = analogRead(BATT_ADC_PIN);

    // fixme
    float battery_voltage = (v / 4095.0) * 2.0 * 3.3 * (_vref / 1000.0);

    return battery_voltage;
}

uint8_t HardwareManager::calcBatteryPercentage(float volts)
{
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