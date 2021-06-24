/******************************************************************************
 * @file
 * @brief Simple GPS continuous fix application for the TDxxxx RF modules.
 * @author Telecom Design S.A.
 * @version 1.2.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014-2016 Telecom Design S.A., http://www.telecomdesign.fr</b>
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
 *   Callback function called a GPS fix has been obtained.
 *
 * @param[in] fix
 *   The GPS fix data structure.
 *
 * @param[in] timeout
 *   Flag that indicates whether a timeout occured if set to true.
 ******************************************************************************/
static void GPSFix(TD_GEOLOC_Fix_t *fix, bool timeout)
{
	int latitude, longitude, latitude_int, latitude_fract, longitude_int,
		longitude_fract;
	char latitude_direction, longitude_direction;
	int i;
	int ephemeris_av = 0;
	int ephemeris_downloading = 0;

	if (fix->type >= TD_GEOLOC_TIME_FIX) {
		tfp_printf("Time (HH:MM:SS): %02d:%02d:%02d\r\n",
			fix->datetime.hours,
			fix->datetime.minutes,
			fix->datetime.seconds);
	}
	if (fix->type >= TD_GEOLOC_DATE_FIX) {
		tfp_printf("Date (DD/MM/YYYY): %02d/%02d/%04d\r\n",
			fix->datetime.day,
			fix->datetime.month,
			fix->datetime.year);
	}
	if (fix->type >= TD_GEOLOC_2D_FIX) {
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
		tfp_printf("Position: %4d.%03d°%c %5d.%03d°%c\r\nSpeed: %d km/h\r\n",
			latitude_int,
			latitude_fract,
			latitude_direction,
			longitude_int,
			longitude_fract,
			longitude_direction,
			fix->speed.speed_kmh);
	}
	if (fix->type >= TD_GEOLOC_3D_FIX) {
		tfp_printf("Altitude: %d m\r\n",
			fix->position.altitude);
	}
	for (i = 0; i < fix->sats.sv_info.channel_count; i++) {
		TD_UBX7_NAV_SVInfo_Channel_t *p = &fix->sats.sv_info.channels[i];
		if (p->flags & 8) {
			ephemeris_av++;
		}
		if ((p->quality > 4) && (!(p->flags & 8))) {
			ephemeris_downloading++;
		}
	}
	tfp_printf("Elapsed: %d s\r\n", fix->duration,fix->sats.usabled);
	tfp_printf("Usabled Sats = %d  -  Ephemeris av. = %d  - Ephemeris Downloading %d\r\n\r\n",
		fix->sats.usabled, ephemeris_av, ephemeris_downloading);
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

	// Set flash variables version
	TD_FLASH_DeleteVariables();

	// Set the device as a Sensor transmitter
	TD_SENSOR_Init(SENSOR_TRANSMITTER, 0, 0);

	// Geoloc and accelerometer initialization
	TD_GEOLOC_Init();
	TD_ACCELERO_Init();

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

	// Process GPS events
	TD_GEOLOC_Process();
}
