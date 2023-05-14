/*********************************************************************/
/*!
*   \file   sensor.c
*
*   \brief  Soil moisture sensor support.
*
*   \author Paweł Majewski
*
*/
/*********************************************************************/
#include "esp_adc_cal.h"
#include "esp_log.h"

#include "sensor.h"

/**********************************************************************
Macros
**********************************************************************/

#define TAG "sensor"

/**********************************************************************
Local Function
**********************************************************************/
/*********************************************************************/
/*!
 * \brief  Converting data into percentages.
 *
 * \param  sample - data converted to percentages.
 * \param  in_min - minimum data input value.
 * \param  inMax - maximum data input value.
 * \param  outMin - minimum data output value.
 * \param  outMax - maximum data output value.
 *
 * \return Value in percent.
 *
 */
/*********************************************************************/
static int8_t mape(int16_t sample, int16_t inMin, int16_t inMax, uint8_t outMin, uint8_t outMax)
{
    return (sample - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

/*********************************************************************/
/*!
 * \brief  Averaging the received data.
 *
 * \param  pData - Pointer where the result is stored.
 *
 * \return None
 *
 */
/*********************************************************************/
static void averageResult(sensorData* pData)
{
    int32_t tempData = 0;
    uint8_t sample = 0;
    uint8_t nrOfAvgValues = 20;

    for(sample = 0; sample < nrOfAvgValues; sample++)
    {
        sensorGetRawData(pData);
        uint16_t value = pData->rawData;
        tempData += value;
    }
    tempData = tempData / nrOfAvgValues;
    pData->averageData = tempData;
}

/**********************************************************************
 Global Function
**********************************************************************/
/*********************************************************************/
/*!
 * \brief  Sensor initialization.
 *
 * \param  None
 *
 * \return None
 *
 */
/*********************************************************************/
void sensorInit(void)
{
    esp_err_t err = ESP_FAIL;

    err = adc1_config_width(ADC_WIDTH_BIT_11);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to configure ADC width: %s", esp_err_to_name(err));
    }

    err = adc1_config_channel_atten( ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure ADC channel attenuation: %s", esp_err_to_name(err));
    }

    ESP_LOGI(TAG, "ADC configuration successful");
}

/*********************************************************************/
/*!
 * \brief  Reading raw data from the sensor.
 *
 * \param  pData - Pointer where the result is stored.
 *
 * \return None
 *
 */
/*********************************************************************/
void sensorGetRawData(sensorData* pData)
{
    pData->rawData = adc1_get_raw(ADC1_CHANNEL_0);
}

/*********************************************************************/
/*!
 * \brief  Converting of averaged data to a voltage value.
 *
 * \param  pData - Pointer where the result is stored.
 *
 * \return None
 *
 */
/*********************************************************************/
void sensorGetVoltageResult(sensorData* pData)
{
    esp_adc_cal_characteristics_t adc_cal;

    esp_adc_cal_characterize( ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_11, 1100, &adc_cal);
    averageResult(pData);
    pData->voltage = esp_adc_cal_raw_to_voltage((pData->averageData), &adc_cal);
}

/*********************************************************************/
/*!
 * \brief  Converting of averaged data to a percentage value.
 *
 * \param  pData - Pointer where the result is stored.
 *
 * \return None
 *
 */
/*********************************************************************/
void sensorGetPercentageResult(sensorData* pData)
{
    uint8_t tempData = 0;

    averageResult(pData);

    if (pData->averageData > 1530)
    {
        pData->percentageResult = 0;
        return;
    }
    if (pData->averageData < 680)
    {
        pData->percentageResult = 100;
        return;
    }

    tempData = mape(pData->averageData, 1530, 680, 0, 100);

    pData->percentageResult = tempData;
}