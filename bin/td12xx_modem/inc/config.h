/******************************************************************************
 * @file
 * @brief Configuration file for the TD12xx/TD15xx RF module firmware
 * @author Telecom Design S.A.
 * @version 3.0.0
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
#ifndef CONFIG_H_
#define CONFIG_H_

#include <td_module.h>

/** Manufacturer */
#define MANUFACTURER			"Telecom Design"

#if MODULE_REVISION == REVISION_TD1202

/** Product */
#define PRODUCT					"TD1202"

#elif MODULE_REVISION == REVISION_TD1208

/** Product */
#define PRODUCT					"TD1208"

#elif MODULE_REVISION == REVISION_TD1204

/** Product */
#define PRODUCT					"TD1204"

#elif MODULE_REVISION == REVISION_TD1205

/** Product */
#define PRODUCT					"TD1205"

#elif MODULE_REVISION == REVISION_TD1508

/** Product */
#define PRODUCT					"TD1508"

#elif MODULE_REVISION == REVISION_TD1205P

/** Product */
#define PRODUCT					"TD1205P"

#elif MODULE_REVISION == REVISION_REF_DESIGN_FCC

/** Product */
#define PRODUCT					"Ref Design FCC"

#elif MODULE_REVISION == REVISION_REF_DESIGN_ETSI

/** Product */
#define PRODUCT					"Ref Design ETSI"

#endif

/** Hardware revision */
#define HARDWARE_VERSION		"0F"

#if MODULE_REVISION == REVISION_TD1202

/** TD1202 Software release and version */
#define SOFTWARE_RELEASE		0000

#elif MODULE_REVISION == REVISION_TD1208

/** TD1208 Software release and version */
#define SOFTWARE_RELEASE		3951

#elif MODULE_REVISION == REVISION_TD1204

/** TD1204 Software release and version */
#define SOFTWARE_RELEASE		3947

#elif MODULE_REVISION == REVISION_TD1205

/** TD1205 Software release and version */
#define SOFTWARE_RELEASE		3948

#elif MODULE_REVISION == REVISION_TD1508

/** TD1508 Software release and version */
#define SOFTWARE_RELEASE		3952

#elif MODULE_REVISION == REVISION_TD1205P

/** TD1205P Software release and version */
#define SOFTWARE_RELEASE		3949

#elif MODULE_REVISION == REVISION_REF_DESIGN_FCC

/** REF_DESIGN_FCC Software release and version */
#define SOFTWARE_RELEASE		3954

#elif MODULE_REVISION == REVISION_REF_DESIGN_ETSI

/** REF_DESIGN_ETSI Software release and version */
#define SOFTWARE_RELEASE		3953

#endif

#define CONCAT(a,b)				a ## b
#define CONCAT2(a,b)			CONCAT(a,b)
#define STRING(x)				#x
#define STRING2(x)				STRING(x)
#define SOFTWARE_VERSION		STRING2(CONCAT2(SOFT,SOFTWARE_RELEASE))

/** Release data */
#define RELEASE_DATE			"M10+2015"

/** Telecom Design 12-digit serial number */
#define SERIAL_NUMBER			"123456789012"

/** TD CORE AT parser extension */
#define AT_CORE					1

#if MODULE_REVISION == REVISION_TD1202

/** TD LAN RF AT parser extension */
#define AT_LAN_RF				0

/** SigFox AT parser extension */
#define AT_PROXY				0

#else

#if MODULE_REVISION == REVISION_TD1508 || MODULE_REVISION == REVISION_REF_DESIGN_FCC
/** SigFox AT parser extension */
#define AT_PROXY				0
#define AT_LAN_RF				0

#else

#define AT_PROXY				1
/** TD LAN RF AT parser extension */
#define AT_LAN_RF				1
#endif

#endif

/** RF chip info AT parser extension */
#define AT_RADIO_INFO			1

/** SigFox AT parser extension */
#define AT_SIGFOX				1


#if MODULE_REVISION == REVISION_TD1202

/** TD Sensor AT parser extension */
#define AT_SENSOR				0

#else

/** TD Sensor AT parser extension */
#define AT_SENSOR				1

#endif


#if MODULE_REVISION == REVISION_TD1204 || MODULE_REVISION == REVISION_TD1205 || MODULE_REVISION == REVISION_TD1205P

/** Geoloc AT parser extension */
#define AT_GEOLOC				1

#else

/** Geoloc AT parser extension */
#define AT_GEOLOC				0

#endif


/* Flash variables version */
#define FLASH_VARIABLES_VERSION	SOFTWARE_RELEASE

#if AT_LAN_RF
#include <at_lan.h>
#else
#define AT_LAN_EXTENSION
#endif

#if AT_RADIO_INFO
#include <at_radio.h>
#else
#define AT_RADIO_EXTENSION
#endif

#if AT_SIGFOX
#include <at_sigfox.h>
#else
#define AT_SIGFOX_EXTENSION
#endif

#if AT_CORE
#include <at_core.h>
#else
#define AT_CORE_EXTENSION
#endif

#if AT_SENSOR
#include <at_sensor.h>
#include <at_sensor_lan.h>
#include <at_sensor_send.h>
#else
#define AT_SENSOR_EXTENSION
#endif

#endif // CONFIG_H_
