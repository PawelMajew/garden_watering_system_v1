/*********************************************************************/
/*!
*   \file   sensor.h
*
*   \brief  Soil moisture sensor support.
*
*   \author Paweł Majewski
*
*/
/*********************************************************************/
#ifndef SENSOR_H
#define SENSOR_H

#include "driver/adc.h"

/**********************************************************************
Data Types
**********************************************************************/
/* Space for read values from the sensor. */
typedef struct
{
    uint16_t rawData;           //Raw data.
    uint16_t averageData;       //Averaged value.
    uint16_t voltage;           //Voltage value.
    uint8_t percentageResult;   //Percentage value.
} sensorData;

/**********************************************************************
Function Declarations
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
void sensorInit(void);

/*********************************************************************/
/*!
 * \brief  Reading raw data from the sensor.
 *
 * \param  data - Pointer where the result is stored.
 *
 * \return None
 *
 */
/*********************************************************************/
void sensorGetRawData(sensorData* pData);

/*********************************************************************/
/*!
 * \brief  Converting of averaged data to a voltage value.
 *
 * \param  data - Pointer where the result is stored.
 *
 * \return None
 *
 */
/*********************************************************************/
void sensorGetVoltageResult(sensorData* pData);

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
void sensorGetPercentageResult(sensorData* pData);

#endif /*SENSOR_H*/