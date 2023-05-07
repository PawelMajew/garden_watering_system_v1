/*********************************************************************/
/*!
*   \file   sensor.c
*
*   \brief  Servo support.
*
*   \author Paweł Majewski
*
*/
/*********************************************************************/
#include <driver/ledc.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"

#include "servo.h"

/**********************************************************************
Macros
**********************************************************************/

#define TAG "Servo"

/**********************************************************************
 Global Function
**********************************************************************/
/*********************************************************************/
/*!
 * \brief  Servo initialization.
 *
 * \param  None
 *
 * \return None
 *
 */
/*********************************************************************/
void servoInit(void)
{
  ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .freq_hz          = 50,
        .clk_cfg          = LEDC_AUTO_CLK
  };
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

  ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = pinServo,
        .duty           = 0,
        .hpoint         = 0
  };
  ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

  ESP_LOGI(TAG, "Init servo finished.");
}

/*********************************************************************/
/*!
 * \brief  Setting the servo mechanism to custom degrees.
 *
 * \param  customData - pulse time.
 *
 * \return None
 *
 */
/*********************************************************************/
void servoDegCustom(float customData)
{
  int duty = (int)(100.0 * (customData / 20.0) * 81.91);

  ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty));
  ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
  vTaskDelay( 1000 / portTICK_PERIOD_MS );
  ESP_ERROR_CHECK(ledc_stop(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0));
}

/*********************************************************************/
/*!
 * \brief  Setting the servo mechanism to 0 degrees.
 *
 * \param  None
 *
 * \return None
 *
 */
/*********************************************************************/
void servoDeg0(void)
{
  servoDegCustom(ServoMsCenter);
}

/*********************************************************************/
/*!
 * \brief  Setting the servo mechanism to 90 degrees.
 *
 * \param  None
 *
 * \return None
 *
 */
/*********************************************************************/
void servoDeg90(void)
{
  servoDegCustom(ServoMsAvg);
}

/*********************************************************************/
/*!
 * \brief  Setting the servo mechanism to 180 degrees.
 *
 * \param  None
 *
 * \return None
 *
 */
/*********************************************************************/
void servoDeg180(void)
{
  servoDegCustom(ServoMsMax);
}