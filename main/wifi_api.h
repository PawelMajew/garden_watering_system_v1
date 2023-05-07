/*********************************************************************/
/*!
*   \file   wifi_api.h
*
*   \brief  File adding extended functionalities for wifi operation.
*
*   \author Paweł Majewski
*
*/
/*********************************************************************/
#ifndef WIFI_API_H
#define WIFI_API_H

#include "sensor.h"

/**********************************************************************
Data Types
**********************************************************************/
/* Data read from JSON. */
typedef struct
{
    float humidity;         //Humidity level.
    int isSensorOn;         //Sensor status (1-on, 0-off).
    int sensorId;           // Sensor ID.

    int wateringProcess;    //Watering status (1-on, 0-off).
    int sprinklerState;     //Manual watering status (1-on, 0-off).
} wifiApi;

/**********************************************************************
Global variables
**********************************************************************/

extern wifiApi wifi_api;

/**********************************************************************
Function Declarations
**********************************************************************/
/*********************************************************************/
/*!
 * \brief  Updating data downloaded from the website.
 *
 * \param  pData - Pointer where the result is stored.
 *
 * \return None
 *
 */
/*********************************************************************/
void getData(char* pData);

/*********************************************************************/
/*!
 * \brief  Preparing JSON with loaded data.
 *
 * \param  pData - Pointer where the result is stored.
 *
 * \return Formatted JSON.
 *
 */
/*********************************************************************/
char* postData(sensorData* pData);

#endif /*WIFI_API_H*/