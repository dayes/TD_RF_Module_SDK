/******************************************************************************
 * @file
 * @brief User Application implementation for TD12xx/TD15xx RF module.
 * @author Telecom Design S.A.
 * @version 2.1.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2012-2015 Telecom Design S.A., http://www.telecomdesign.fr</b>
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
 *****************************************************************************/

#include "config.h"

#include <efm32.h>

#include <td_module.h>
#include <td_core.h>
#include <td_flash.h>
#include <td_uart.h>
#include <td_printf.h>
#include <td_stream.h>
#include <at_parse.h>

#if AT_CORE
#include <at_core.h>
#endif

#if AT_LAN_RF
#include <at_lan.h>
#include <td_lan.h>
#endif
#if AT_RADIO_INFO
#include <at_radio.h>
#endif
#if AT_SIGFOX
#include <at_sigfox.h>
#endif
#if AT_PROXY
#include <at_proxy.h>
#endif

#if AT_SENSOR
#include <at_sensor.h>
#include <at_sensor_lan.h>
#include <at_sensor_send.h>
#endif

#if AT_GEOLOC
#include <at_geoloc.h>
#include <at_accelero.h>
#endif

#include <td_sigfox.h>

/* This file declare all "dynamic" library data. It should be last included file
 * Standard size value can be override before including this file
 */
#if MODULE_REVISION == REVISION_TD1202
#define TD_ALL_DUMP_REMOVE_CODE
#endif

#if AT_SENSOR
#define TD_SENSOR_GATEWAY_MAX_DEVICE 5
#endif

#define TD_SIGFOX_USE_TEST_CARRIER_CODE		// this one can be removed
#define TD_SIGFOX_USE_DOWNLINK_TEST_CODE
//#define TD_SCHEDULER_REMOVE_CODE
#define TD_TRAP_RESET_CODE
#define PRODUCT_LED_BLINK 1
//#define TD_SIGFOX_TRANSMIT_FORWARD_ONLY
//#define TD_SIGFOX_TRANSMIT_LOCAL_ONLY
//#define TD_SIGFOX_TRANSMIT_NONE
//#define TD_SIGFOX_REMOVE_CODE
#if AT_PROXY == 0
#define TD_SIGFOX_PROXY_REMOVE_CODE
#endif

#if AT_LAN_RF == 0
#define TD_SENSOR_GATEWAY_REMOVE_CODE
#define TD_SENSOR_LAN_REMOVE_CODE
#endif
//#define TD_SIGFOX_DOWNLINK_REMOVE_CODE
//#define TD_SIGFOX_RECEIVE_NONE
//#define PRODUCT_EARLY_DEBUG
#include <td_config.h>

/*******************************************************************************
 **************************   PUBLIC FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   User setup function.
 ******************************************************************************/
void TD_USER_Setup(void)
{
	TD_UART_Options_t options = {LEUART_DEVICE, LEUART_LOCATION, 9600, 8, 'N',
		1, false};

	// Open an I/O stream using LEUART0
	TD_UART_Open(&options, TD_STREAM_RDWR);

	// Define variables version to avoid wrong flash init
	TD_FLASH_SetVariablesVersion(FLASH_VARIABLES_VERSION);
#if AT_CORE
	AT_AddExtension(&core_extension);
#endif
#if AT_RADIO_INFO
	AT_AddExtension(&radio_extension);
#endif
#if AT_SIGFOX
	AT_AddExtension(&sigfox_extension);
#endif
#if AT_PROXY
	AT_AddExtension(&proxy_extension);
#endif
#if AT_LAN_RF
	AT_AddExtension(&lan_extension);
#endif

#if AT_SENSOR
   	AT_AddExtension(&sensor_extension);
	AT_AddExtension(&sensor_lan_extension);
	AT_AddExtension(&sensor_send_extension);
#endif
#if AT_GEOLOC
	AT_AddExtension(&geoloc_extension);
	AT_AddExtension(&accelero_extension);

#endif
	// Initialize the AT command parser
	AT_Init();
#if AT_SENSOR

    //Initialize Sensor
    TD_SENSOR_Init(TD_SENSOR_GetModuleType(), TD_LAN_GetFrequency(),
    	TD_LAN_GetPowerLevel());
    TD_SENSOR_SetModuleApplicationRelease(SOFTWARE_RELEASE);
#endif

#if AT_GEOLOC

    // Initialize GPS
    TD_GEOLOC_Init();

    // Initialize Accelero
   TD_ACCELERO_Init();
#endif
}

/***************************************************************************//**
 * @brief
 *   User loop function.
 ******************************************************************************/
void TD_USER_Loop(void)
{
	int c;

#if AT_LAN_RF
	// Process local RF
	TD_LAN_Process();
#endif

#ifdef AT_SIGFOX
	TD_SIGFOX_DOWNLINK_Process();
#endif

#if AT_SENSOR
	TD_SENSOR_Process();
#endif

#if AT_GEOLOC
	TD_GEOLOC_Process();
	TD_ACCELERO_Process();
#endif
	while ((c = TD_UART_GetChar()) >= 0) {
		AT_Parse(c);
	}
}
