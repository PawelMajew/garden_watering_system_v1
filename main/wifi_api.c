/*********************************************************************/
/*!
*   \file   wifi_api.c
*
*   \brief  File adding extended functionalities for wifi operation.
*
*   \author Paweł Majewski
*
*/
/*********************************************************************/
#include "cJSON.h"

#include "wifi_api.h"

/**********************************************************************
Macros
**********************************************************************/

#define BOARD 0

#if BOARD == 0
    #define SENSOR_NUMBER 0
    #define TEMP_JSON "{ \"sensor_id\": 1, \"humidity\": 28, \"is_sensor_on\": 1}"
#else
    #define SENSOR_NUMBER 1
    #define TEMP_JSON "{ \"sensor_id\": 2, \"humidity\": 28, \"is_sensor_on\": 1}"
#endif

/**********************************************************************
Global variables
**********************************************************************/

wifiApi wifi_api;

/**********************************************************************
Global Function
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
void getData(char* pData)
{
    cJSON* pRoot = cJSON_Parse(pData);
    if (pRoot == NULL)
    {
        return;
    }
    cJSON* pSensorData = cJSON_GetObjectItem(pRoot, "sensor_data");
    int num_sensors = cJSON_GetArraySize(pSensorData);

    if (num_sensors > 0)
    {
        cJSON* pSensor = cJSON_GetArrayItem(pSensorData, SENSOR_NUMBER);
        wifi_api.humidity = cJSON_GetObjectItem(pSensor, "humidity")->valuedouble;
        wifi_api.isSensorOn = cJSON_GetObjectItem(pSensor, "is_sensor_on")->valueint;
        wifi_api.sensorId = cJSON_GetObjectItem(pSensor, "sensor_id")->valueint;
        wifi_api.wateringProcess = cJSON_GetObjectItem(pRoot, "watering_process")->valueint;
        wifi_api.sprinklerState = cJSON_GetObjectItem(pRoot, "sprinkler_state")->valueint;
    }
    cJSON_Delete(pRoot);
}

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
char* postData(sensorData *pData)
{
    char* pJsonData = TEMP_JSON;
    cJSON* pRoot = cJSON_Parse(pJsonData);
    cJSON_ReplaceItemInObject(pRoot, "humidity", cJSON_CreateNumber(pData->percentageResult));
    char* pNewJsonData = cJSON_Print(pRoot);

    cJSON_Delete(pRoot);
    return pNewJsonData;
}