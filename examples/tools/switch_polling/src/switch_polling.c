/******************************************************************************
 * @file
 * @brief Switch polling demonstration application for the TDxxxx RF modules.
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

 #include <efm32.h>

#include <td_core.h>
#include <td_rtc.h>
#include <td_uart.h>
#include <td_printf.h>
#include <td_stream.h>
#include <td_flash.h>

#include <td_tools_switch.h>

#define MANUFACTURER			"TD"
#define PRODUCT					"Switch polling"
#define HARDWARE_VERSION		0
#define SOFTWARE_VERSION		0
#define RELEASE_DATE			0
#define SERIAL_NUMBER			0

#define TD_GEOLOC_USE_CODE		0
#define TD_SCHEDULER_MAX_TIMER	5
#include <td_config.h>

static uint8_t SwitchId;

/**
 * @brief  User setup function
 **/
void TD_USER_Setup(void)
{
	TD_UART_Options_t options = {LEUART_DEVICE, LEUART_LOCATION, 9600, 8, 'N',
		1, false};

	// Open an I/O stream using LEUART0
	TD_UART_Open(&options, TD_STREAM_RDWR);

	// If using DB2/DB3
  	GPIO_DbgSWDIOEnable(false);
    GPIO_DbgSWDClkEnable(false);

	// Push button initialization
	SwitchId = TD_TOOLS_SWITCH_Init(gpioPortF, 1, gpioModeInputPull, true, true,
		2, 5, 8);

	// On / Off button initialization
	// SwitchId = TD_TOOLS_SWITCH_Init(gpioPortF, 1, gpioModeInputPull, true,
	// false, 2, 5, 8);

	// Interrupt handling
	 TD_TOOLS_SWITCH_Start(SwitchId);

	// 1 s period polling handling
	//TD_TOOLS_SWITCH_StartPolling(SwitchId, T1S);
}

/**
 * @brief  User loop function
 **/
void TD_USER_Loop(void)
{
	// Print event
	switch (TD_TOOLS_SWITCH_Process(SwitchId)) {
	case TD_TOOLS_SWITCH_ON:
		tfp_printf("TD_TOOLS_SWITCH_ON\r\n");
		break;

	case TD_TOOLS_SWITCH_OFF:
		tfp_printf("TD_TOOLS_SWITCH_OFF\r\n");
		break;

	case TD_TOOLS_SWITCH_STARTUP_PUSHED:
		tfp_printf("TD_TOOLS_SWITCH_STARTUP_PUSH\r\n");
		break;

	case TD_TOOLS_SWITCH_SHORT_PUSHED:
		tfp_printf("TD_TOOLS_SWITCH_SHORT_PUSHED\r\n");
		break;

	case TD_TOOLS_SWITCH_SHORT_RELEASED:
		tfp_printf("TD_TOOLS_SWITCH_SHORT_RELEASED\r\n");
		break;

	case TD_TOOLS_SWITCH_LONG_PUSHED:
		tfp_printf("TD_TOOLS_SWITCH_LONG_PUSHED\r\n");
		break;

	case TD_TOOLS_SWITCH_LONG_RELEASED:
		tfp_printf("TD_TOOLS_SWITCH_LONG_RELEASED\r\n");
		break;

	case TD_TOOLS_SWITCH_VERY_LONG_PUSHED:
		tfp_printf("TD_TOOLS_SWITCH_VERY_LONG_PUSHED\r\n");
		break;

	case TD_TOOLS_SWITCH_VERY_LONG_RELEASED:
		tfp_printf("TD_TOOLS_SWITCH_VERY_LONG_RELEASED\r\n");
		break;

	case TD_TOOLS_SWITCH_EXTRA_LONG_PUSHED:
		tfp_printf("TD_TOOLS_SWITCH_EXTRA_LONG_PUSHED\r\n");
		break;

	case TD_TOOLS_SWITCH_EXTRA_LONG_RELEASED:
		tfp_printf("TD_TOOLS_SWITCH_EXTRA_LONG_RELEASED\r\n");
		break;

	case TD_TOOLS_SWITCH_DOUBLE_CLICK:
		tfp_printf("TD_TOOLS_SWITCH_DOUBLE_CLICK\r\n");
		break;

	case TD_TOOLS_SWITCH_TRIPLE_CLICK:
		tfp_printf("TD_TOOLS_SWITCH_TRIPLE_CLICK\r\n");
		break;

	case TD_TOOLS_SWITCH_MULTIPLE_CLICK_PENDING:
		tfp_printf("TD_TOOLS_SWITCH_MULTIPLE_CLICK_PENDING %d\r\n",
			TD_TOOLS_SWITCH_GetMultipleClickCount(SwitchId));
		break;

	case TD_TOOLS_SWITCH_MULTIPLE_CLICK_DONE:
		tfp_printf("TD_TOOLS_SWITCH_MULTIPLE_CLICK_DONE %d\r\n",
			TD_TOOLS_SWITCH_GetMultipleClickCount(SwitchId));
		break;

	default:
		break;
	}
}
