/******************************************************************************
 * @file
 * @brief Simple GPS periodic fix application for the TDxxxx RF modules.
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

#include <stdint.h>
#include <stdbool.h>

#include <efm32.h>

#include <td_core.h>
#include <td_uart.h>
#include <td_printf.h>
#include <td_stream.h>
#include <td_flash.h>
#include <td_scheduler.h>
#include <td_watchdog.h>

#include <td_sensor.h>
#include <sensor_data_geoloc.h>

#include <td_accelero.h>
#include <td_geoloc.h>

#include <td_config.h>

/*******************************************************************************
 ******************************  DEFINES ****************************
 ******************************************************************************/

/** Flash variable version ID */
#define VARIABLES_VERSION 0

/** Stop trying to fix after timeout (in seconds) */
#define FIX_TIMEOUT 120

// Interval at which position is reported (in seconds)
#define FIX_INTERVAL 300

/** Acceptable minimum horizontal accuracy, 800 to be very accurate */
#define FIX_HDOP 800

/** Boot monitoring, 1 to enable */
#define BOOT_MONITORING 0

/** Keepalive monitoring interval in hours, 0 to disable */
#define KEEPALIVE_INTERVAL 0

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
static void GPSFix(TD_GEOLOC_Fix_t * fix, bool timeout)
{
	int latitude, longitude, latitude_int, latitude_fract, longitude_int,
		longitude_fract;
	char latitude_direction, longitude_direction;

	if (fix->type >= TD_GEOLOC_2D_FIX && fix->hard.rtc_calibrated) {
		latitude = fix->position.latitude;
		if (latitude < 0) {
			latitude_direction = 'S';
			latitude = -latitude;
		} else {
			latitude_direction = 'N';
		}
		latitude_int = latitude / 10000000;
		latitude_fract = latitude % 10000000;
		longitude = fix->position.longitude;
		if (longitude < 0) {
			longitude_direction = 'W';
			longitude = -longitude;
		} else {
			longitude_direction = 'E';
		}
		longitude_int = longitude / 10000000;
		longitude_fract = longitude % 10000000;
		tfp_printf("Fix OK: %4d.%03d°%c %5d.%03d°%c\r\n",
			latitude_int,
			latitude_fract,
			latitude_direction,
			longitude_int,
			longitude_fract,
			longitude_direction);
		TD_GEOLOC_StopFix(TD_GEOLOC_HW_BCKP);
		TD_SENSOR_SendDataPosition(GPS_DATA_XYZ_SV_HDOP, fix, 0, 0);
		tfp_printf("done");

	} else if (timeout) {
		tfp_printf("Fix Timeout\r\n");
		TD_GEOLOC_StopFix(TD_GEOLOC_HW_BCKP);
		TD_SENSOR_SendDataPosition(GPS_DATA_XYZ_SV_HDOP, fix, 0, 0);
	}
}

/***************************************************************************//**
 * @brief
 *  Start fixing periodically.
 *
 * @param[in] arg
 *  Generic argument set by the user that is passed along to the callback
 *  function.
 *
 * @param[in] repeat_count
 *  Updated repeat count, decremented at each timer trigger, unless it is an
 *  infinite timer.
 ******************************************************************************/
static void StartFixing(uint32_t arg, uint8_t repeat_count)
{
	tfp_printf("Start fixing\r\n");
	TD_GEOLOC_TryToFix(TD_GEOLOC_NAVIGATION, FIX_TIMEOUT, GPSFix);
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

	// Use a 64 s automatic watchdog
	TD_WATCHDOG_Init(64);
	TD_WATCHDOG_Enable(true, true);
	TD_SENSOR_Init(SENSOR_TRANSMITTER, 0, 0);

	// Geoloc and accelerometer initialization
	TD_GEOLOC_Init();
	TD_ACCELERO_Init();

#if BOOT_MONITORING

	// Will only send a boot monitor frame on NEXT reboot
	TD_SENSOR_MonitorBoot(true, 0);
#endif

#if KEEPALIVE_INTERVAL > 0

	// Send a keep-alive frame immediately, then at given interval
	TD_SENSOR_MonitorKeepAlive(true, KEEPALIVE_INTERVAL);
#endif

	// Start fixing right now
	StartFixing(0, 0);

	// Start the fix infinite timer
	TD_SCHEDULER_Append(FIX_INTERVAL, 0, 0, TD_SCHEDULER_INFINITE, StartFixing,
		0);
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
