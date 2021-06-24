/******************************************************************************
 * @file
 * @brief TD LAN RX example for TDxxxx RF modules.
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
  ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include <efm32.h>

#include <em_gpio.h>

#include <td_core.h>
#include <td_uart.h>
#include <td_printf.h>
#include <td_stream.h>
#include <td_rtc.h>
#include <td_utils.h>

#include <td_lan.h>

/* This file declare all "dynamic" library data. It should be last included file
 * Standard size value can be override before including this file
 */
#define FORCE_RADIO_RESET 0
#define TD_SENSOR_USE_CODE 0
#define TD_GEOLOC_USE_CODE 0
#include <td_config.h>

/*******************************************************************************
 *************************   DEFINES   *****************************************
 ******************************************************************************/

#define LED_PORT		TIM2_PORT			/**< LED port */
#define LED_BIT			TIM2_BIT			/**< LED bit */

#define NODE_ADDRESS	0x00000000			/**< Node address */
#define NODE_NETWORK	0x00200000			/**< Node network */
#define	NODE_MASK		0x00FF0000			/**< Node mask */
#define RETRIES			2					/**< Number of transmit retries */

#define FREQUENCY_ETSI		869000000			/**< Channel frequency */
#define POWER_LEVEL_ETSI    14					/**< Transmit power level */

#define FREQUENCY_FCC		905000000			/**< Channel frequency */
#define POWER_LEVEL_FCC	    -2					/**< Transmit power level */



/*******************************************************************************
 ******************************  CONSTANTS  ************************************
 ******************************************************************************/

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
    int i, j;
    TD_LAN_frame_t RX, TX;
    // int delay;

	// Open an I/O stream using LEUART0
	TD_UART_Open(&options, TD_STREAM_RDWR);

    // Initialize the LAN
    if (TD_LAN_Init(true, (NODE_ADDRESS | NODE_NETWORK), NODE_MASK) == false) {
    	printf("Problem in init\r\n");
	}

    // Set the operating frequency and power level
    if (CONFIG_ITU_ISM_REGION == 1) {
		if (TD_LAN_SetFrequencyLevel(FREQUENCY_ETSI, POWER_LEVEL_ETSI) == false) {
			printf("Problem while setting frequency/level\r\n");
		}
    } else if (CONFIG_ITU_ISM_REGION == 2) {
    	if (TD_LAN_SetFrequencyLevel(FREQUENCY_FCC, POWER_LEVEL_FCC) == false) {
    		printf("Problem while setting frequency/level\r\n");
    	}
    } else {
    	printf("ERROR with CONFIG_ITU_ISM_REGION.\r\n"
    		   "Must be equal to 1 (ETSI) or 2 (FCC).\r\n");
    	return;
    }

    for (i = 1; i < 1000000; i++) {
    	if (TD_LAN_ReceiveFrameSync(&RX)) {

    		// Reply frame to sender with acknowledge flag set
    		TX.header = RX.header;
    		SET_ACK(TX.header, TD_LAN_ACK);
    		for (j = 0; j < TD_LAN_PAYLOAD_SIZE; j++) {
    			TX.payload[j] = RX.payload[j];
    		}
    		TD_LAN_SendFrame(1, &TX, &RX);
    		tfp_dump("FRAME=", (uint8_t *) &RX, TD_LAN_FRAME_SIZE);
    	}

        // Random delay from 0 up to 2S
        //delay = rand() % 2000;
    	//delay = 200;
    	// TD_RTC_Delay(delay * T1MS + T500MICROS);
	}

    // Never reached
}

/***************************************************************************//**
 * @brief
 *   User loop function.
 ******************************************************************************/
void TD_USER_Loop(void)
{
	// Never called
}
