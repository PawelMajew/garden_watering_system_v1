/*********************************************************************/
/*!
*   \file   main.c
*
*   \brief  MAIN.
*
*   \author Paweł Majewski
*
*/
/*********************************************************************/
#include "wifi.h"
#include "sensor.h"
#include "leds.h"
#include "wifi_api.h"
#include "servo.h"
#include "task.h"

#define BOARD 0

void app_main(void)
{
    wifiInit();
    ledsGpioInit();
    sensorInit();
#if BOARD == 0
    servoInit();
#endif

    xTaskCreatePinnedToCore(taskSensor, "Task_sensor", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(taskWifi, "Task_wifi", 4096, NULL, 1, NULL, 1);
#if BOARD == 0
    xTaskCreatePinnedToCore(taskSprinklers, "Task_Sprinklers", 4096, NULL, 2, NULL, 1);
#endif
}