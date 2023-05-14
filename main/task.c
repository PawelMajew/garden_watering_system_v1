/*********************************************************************/
/*!
*   \file   task.c
*
*   \brief  Task support.
*
*   \author Paweł Majewski
*
*/
/*********************************************************************/

#include "esp_log.h"

#include "wifi.h"
#include "sensor.h"
#include "leds.h"
#include "wifi_api.h"
#include "servo.h"

#include "task.h"


/**********************************************************************
Macros
**********************************************************************/

/* Time when we are not irrigating. */
#define NORMAL_MEASURMENT_TIME 1000 //* 60 * 60 / 2
/* Time when we are irrigating. */
#define WATERING_MEASURMENT_TIME 1000 //TIME_WATERING_1 + TIME_PAUSE_1 + TIME_WATERING_2 + TIME_PAUSE_2
/* Time when we are irrigating manual. */
#define MANUAL_WATERING_MEASURMENT_TIME 1000 //* 60 
/* How often data will be downloaded from the site. */
#define GET_DELAY 1000

/* Watering sequence times. */
#define TIME_WATERING_1 1000 //* 60 * 2
#define TIME_PAUSE_1 1000 //* 60 * 10
#define TIME_WATERING_2 1000 //* 60 * 1
#define TIME_PAUSE_2 1000 //* 60 * 5

#define TRUE 1
#define FALSE 0

/**********************************************************************
Local Function
**********************************************************************/
/*********************************************************************/
/*!
 * \brief  Lighting up different LEDs depending on hydration status.
 *
 * \param  pData - Pointer where the result is stored.
 *
 * \return None
 *
 */
/*********************************************************************/
static void taskLedStatus(sensorData *pData)
{
    turnOffLed(lowHydrationStatus);
    turnOffLed(moderateHydrationStatus);
    turnOffLed(goodHydrationStatus);

    if (pData->percentageResult <= 25)
    {
        turnOnLed(lowHydrationStatus);
    }
    else if (pData->percentageResult >= 25 && pData->percentageResult <= 75)
    {
        turnOnLed(moderateHydrationStatus);
    }
    else if (pData->percentageResult >= 75)
    {
        turnOnLed(goodHydrationStatus);
    }
}

/**********************************************************************
Global Function
**********************************************************************/
/*********************************************************************/
/*!
 * \brief  Reading/sending data from the sensor to the rest api
 *         and visualization of the soil condition based on LEDs.
 *
 * \param  pvParameters - Pointer that will be used as the parameter for the task being created.
 *
 * \return None
 *
 */
/*********************************************************************/
void taskSensor(void *pvParameters) {
    sensorData data;

    while (TRUE) {
        sensorGetPercentageResult(&data);
        restPost(&data);
        taskLedStatus(&data);

        if ( wifi_api.sprinklerState == TRUE)
        {
            vTaskDelay(MANUAL_WATERING_MEASURMENT_TIME / portTICK_PERIOD_MS);
        }
        else if (wifi_api.wateringProcess == TRUE)
        {
            vTaskDelay(WATERING_MEASURMENT_TIME / portTICK_PERIOD_MS);
        }
        else
        {
            vTaskDelay(NORMAL_MEASURMENT_TIME / portTICK_PERIOD_MS);
        }

    }
}

/*********************************************************************/
/*!
 * \brief  Reading data from the page.
 *
 * \param  pvParameters - Pointer that will be used as the parameter for the task being created.
 *
 * \return None
 *
 */
/*********************************************************************/
void taskWifi(void *pvParameters) {
    while (TRUE) {
        restGet();
        vTaskDelay(GET_DELAY / portTICK_PERIOD_MS);
    }
}

/*********************************************************************/
/*!
 * \brief  Sprinklers support.
 *
 * \param  pvParameters - Pointer that will be used as the parameter for the task being created.
 *
 * \return None
 *
 */
/*********************************************************************/
#if BOARD == 0
void taskSprinklers(void *pvParameters)
{
    while (TRUE)
    {
        /* Automatic watering. */
        if (wifi_api.wateringProcess == TRUE && wifi_api.sprinklerState == FALSE)
        {
            turnOnLed(servoStatus);

            /* Step one: first watering. */
            servoDeg180();
            vTaskDelay(TIME_WATERING_1 / portTICK_PERIOD_MS);

            /* Step two: waiting for the water to absorb. */
            servoDeg0();
            vTaskDelay(TIME_PAUSE_1 / portTICK_PERIOD_MS);

            /* Step three: second watering. */
            servoDeg180();
            vTaskDelay(TIME_WATERING_2 / portTICK_PERIOD_MS);

            /* Step four: waiting for the water to absorb. */
            servoDeg0();
            vTaskDelay(TIME_PAUSE_2 / portTICK_PERIOD_MS);

            turnOffLed(servoStatus);
        }

        /* Manual opening of the valve. */
        if (wifi_api.sprinklerState == TRUE)
        {
            servoDeg180();
            turnOnLed(servoStatus);
        }
        else
        {
            servoDeg0();
            turnOffLed(servoStatus);
        }

        /* Half-second response time delay. */
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
#endif