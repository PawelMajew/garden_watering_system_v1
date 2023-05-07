/*********************************************************************/
/*!
*   \file   leds.h
*
*   \brief  Led service.
*
*   \author Paweł Majewski
*
*/
/*********************************************************************/
#ifndef LEDS_H
#define LEDS_H

/**********************************************************************
Macros
**********************************************************************/

#define LED_ON 1
#define LED_OFF 0

#define MAX_LED  6
#define LED_PIN_32 32
#define LED_PIN_33 33
#define LED_PIN_25 25
#define LED_PIN_26 26
#define LED_PIN_27 27

/**********************************************************************
Data Types
**********************************************************************/
/* The role of LEDs in showing the status. */
typedef enum
{
    lowHydrationStatus = LED_PIN_32,        // low soil hydration red diode
    moderateHydrationStatus = LED_PIN_33,   // moderate soil hydration yellow diode
    goodHydrationStatus = LED_PIN_25,       // good soil hydration green diode
    servoStatus = LED_PIN_26,               // on - valve open, off - valve close
    wifiUiStatus = LED_PIN_27,              // on - connected, off - not connected (wifi)
}ledRole;

/**********************************************************************
Function Declarations
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
void ledsGpioInit(void);

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
void turnOnLed(ledRole led);

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
void turnOffLed(ledRole led);

#endif