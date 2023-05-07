/*********************************************************************/
/*!
*   \file   servo.h
*
*   \brief  Servo support.
*
*   \author Paweł Majewski
*
*/
/*********************************************************************/
#ifndef SERVO_H
#define SERVO_H

/**********************************************************************
Macros
**********************************************************************/

#define pinServo 22
#define ServoMsMin 0.06     // -90 degrees.
#define ServoMsCenter 1.5   // 0 degrees.
#define ServoMsMax 2.1      // 90 degrees.
#define ServoMsAvg ((ServoMsMax-ServoMsMin)/2.0)

/**********************************************************************
Function Declarations
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
void servoInit(void);

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
void servoDegCustom(float customData);

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
void servoDeg0(void);

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
void servoDeg90(void);

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
void servoDeg180(void);

#endif /*SERVO_H*/