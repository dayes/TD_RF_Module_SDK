/******************************************************************************
 * @file
 * @brief SIGFOX downlink example for TDxxxx RF modules.
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

#include <stdint.h>
#include <stdbool.h>

#include <efm32.h>
#include <em_gpio.h>

#include <td_core.h>
#include <td_rtc.h>
#include <td_uart.h>
#include <td_gpio.h>
#include <td_printf.h>
#include <td_stream.h>

#include <td_sigfox.h>

/* This file declare all "dynamic" library data. It should be last included file
 * Standard size value can be override before including this file
 */
#define RADIO_USE_TCXO 1
#define TD_SENSOR_USE_CODE 0
#define TD_GEOLOC_USE_CODE 0
#include <td_config.h>

/*******************************************************************************
 *************************   DEFINES   *****************************************
 ******************************************************************************/

#define LED_PORT		TIM2_PORT			/**< LED port */
#define LED_BIT			TIM2_BIT			/**< LED bit */

/*******************************************************************************
 ******************************  CONSTANTS  ************************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Call-back function for SIGFOX down-link.
 *
 * @param[in] rx_frame
 *   Pointer to the received frame or null if timeout occurred.
 *
 * @param[in] length
 *   Length in bytes of the received frame.
 *
 * @return
 *   Returns the buffer length if OK, -1 if an error occurred.
 ******************************************************************************/
static int downlink_callback(uint8_t *rx_frame, uint8_t length)
{
	if (rx_frame == 0) {

		// Finished receiving
		TD_SIGFOX_DOWNLINK_SetUserCallback(0);
		tfp_printf("RX END\r\n");

		// Done
		return 1;
	} else {
		if (length == 0) {

			// Start receiving
			tfp_printf("RX BEGIN\r\n");

			// Done
			return 1;
		}

		// Received one good frame
		tfp_dump("RX=", rx_frame, length);

		// Done
		return 1;
	}
}

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
	int i;
	uint8_t message[12];

	// Open an I/O stream using LEUART0
	TD_UART_Open(&options, TD_STREAM_RDWR);

    // Define the LED pin as an output in push-pull mode
	GPIO_PinModeSet(LED_PORT, LED_BIT, gpioModePushPull, 0);

	// Register down-link callback function
	TD_SIGFOX_DOWNLINK_SetUserCallback(downlink_callback);

   // Build hex message
   for (i = 0; i < 12; i++) {
	   message[i] = i;
   }

   // Send hex message with 2 repetitions
   GPIO_PinOutSet(LED_PORT, LED_BIT);
   tfp_printf("TX BEGIN\r\n");
   TD_SIGFOX_SendV1(MODE_FRAME, false, message, 12, 2, true, false);
   GPIO_PinOutClear(LED_PORT, LED_BIT);
   tfp_dump("TX=", message, 12);
   tfp_printf("TX END\r\n");
}

/***************************************************************************//**
 * @brief
 *   User loop function.
 ******************************************************************************/
void TD_USER_Loop(void)
{
	// Process down-link events
	TD_SIGFOX_DOWNLINK_Process();
}
