/******************************************************************************
 * @file
 * @brief Simple GPS periodic NMEA output application for the TDxxxx RF modules.
 * @author Telecom Design S.A.
 * @version 1.1.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014-2015 Telecom Design S.A., http://www.telecomdesign.fr</b>
 ******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Telecom Design SA has no
 * obligation to support this Software. Telecom Design SA is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Telecom Design SA will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
  ******************************************************************************/

#include "config.h"

#include <efm32.h>

#include <td_core.h>
#include <td_uart.h>
#include <td_printf.h>
#include <td_stream.h>
#include <td_flash.h>

#include <td_sensor.h>

#include <td_accelero.h>
#include <td_geoloc.h>
#include <nmea_parser.h>

#include <td_config.h>

/*******************************************************************************
 ******************************  GLOBAL FUNCTIONS  ****************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *  GPS fix callback
 *
 * @param[in] fix
 *   The GPS fix data structure.
 *
 * @param[in] timeout
 *   Flag that indicates whether a timeout occurred if set to true.
 ******************************************************************************/
static void GPSFix(TD_GEOLOC_Fix_t *fix, bool timeout)
{
}

/***************************************************************************//**
 * @brief
 *  User Setup function.
 ******************************************************************************/
void TD_USER_Setup(void)
{
	TD_UART_Options_t options = {LEUART_DEVICE, LEUART_LOCATION, 9600, 8, 'N',
		1, false};

	// Open an I/O stream using LEUART0
	TD_UART_Open(&options, TD_STREAM_RDWR);

	// No Flash variable
	TD_FLASH_DeleteVariables();

	// Sensor as transmitter
	TD_SENSOR_Init(SENSOR_TRANSMITTER, 0, 0);

	// Geoloc and accelerometer initialization
	TD_GEOLOC_Init();
	TD_ACCELERO_Init();

	// Enable NMEA output, output everything
	TD_NMEA_EnableOutput(true, "*");

	// Start GPS forever
	TD_GEOLOC_TryToFix(TD_GEOLOC_NAVIGATION, TD_GEOLOC_INFINITE, GPSFix);
}

/***************************************************************************//**
 * @brief
 *   User loop function.
 ******************************************************************************/
void TD_USER_Loop(void)
{
	// Process Sensor events
	TD_SENSOR_Process();

	// Process geoloc events
	TD_GEOLOC_Process();
}
