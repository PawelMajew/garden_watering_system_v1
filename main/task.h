/*********************************************************************/
/*!
*   \file   task.h
*
*   \brief  Task support.
*
*   \author Paweł Majewski
*
*/
/*********************************************************************/
#ifndef TASK_H
#define TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#define BOARD 0

/**********************************************************************
Function Declarations
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
void taskSensor(void *pvParameters);

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
void taskWifi(void *pvParameters);

/*********************************************************************/
/*!
 * \brief  Servo support.
 *
 * \param  pvParameters - Pointer that will be used as the parameter for the task being created.
 *
 * \return None
 *
 */
/*********************************************************************/
#if BOARD == 0
void taskSprinklers(void *pvParameters);
#endif

#endif /*TASK_H*/