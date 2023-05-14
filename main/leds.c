/*********************************************************************/
/*!
*   \file   leds.c
*
*   \brief  Led service.
*
*   \author Paweł Majewski
*
*/
/*********************************************************************/
#include "driver/gpio.h"
#include "esp_log.h"

#include "leds.h"

/**********************************************************************
Macros
**********************************************************************/

#define TAG "leds"

/**********************************************************************
Global Function
**********************************************************************/
/*********************************************************************/
/*!
 * \brief  Initialization of selected LEDs
 *
 * \param  None
 *
 * \return None
 *
 */
/*********************************************************************/
void ledsGpioInit(void)
{
    esp_err_t err = ESP_OK;

    esp_rom_gpio_pad_select_gpio(lowHydrationStatus);
    err += gpio_set_direction(lowHydrationStatus, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(moderateHydrationStatus);
    err += gpio_set_direction(moderateHydrationStatus, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(goodHydrationStatus);
    err += gpio_set_direction(goodHydrationStatus, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(servoStatus);
    err += gpio_set_direction(servoStatus, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(wifiUiStatus);
    err += gpio_set_direction(wifiUiStatus, GPIO_MODE_OUTPUT);

    if (err == 0)
    {
        ESP_LOGI(TAG, "LEDS configuration successful");
    }
    else
    {
        ESP_LOGE(TAG, "LEDS configuration fail");
    }
}

/*********************************************************************/
/*!
 * \brief  Turn on the selected diode
 *
 * \param  led - selected diode
 *
 * \return None
 *
 */
/*********************************************************************/
void turnOnLed(ledRole led)
{
    gpio_set_level(led, LED_ON);
}

/*********************************************************************/
/*!
 * \brief  Turn off the selected diode
 *
 * \param  led - selected diode
 *
 * \return None
 *
 */
/*********************************************************************/
void turnOffLed(ledRole led)
{
    gpio_set_level(led, LED_OFF);
}