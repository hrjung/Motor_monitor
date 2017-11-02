/******************************************************************************

 @file  accelerometer.h

 @brief This file contains Accelerometer Profile header file.

 Group: WCS, BTS
 Target Device: CC2650, CC2640, CC1350

 ******************************************************************************
 
 Copyright (c) 2009-2016, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 Release Name: ble_sdk_2_02_00_31
 Release Date: 2016-06-16 18:57:29
 *****************************************************************************/

#ifndef MOTOR_MONIOR_H
#define MOTOR_MONIOR_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// Profile Parameters
// RW uint8 - Profile Attribute value
#define MONITOR_FREQ				0
#define MONITOR_RPM				  	1
#define MONITOR_BATTERY				2
#define MONITOR_DISTANCE			3
  
// Profile UUIDs
#define MONITOR_FREQ_UUID		0xFFE2
#define MONITOR_RPM_UUID		0xFFE3
#define MONITOR_BATTERY_UUID	0xFFE4
#define MONITOR_DISTANCE_UUID  	0xFFE5
  
// Monitor Service UUID
#define MONITOR_SERVICE_UUID            0xFFE0
  
// Monitor Profile Services bit fields
#define MONITOR_SERVICE               0x00000001

// Values for flags
#define MONITOR_FLAGS_NONE		0
#define MONITOR_FLAGS_FREQ		1
#define MONITOR_FLAGS_RPM		2
#define MONITOR_FLAGS_VOLT		3
#define MONITOR_FLAGS_RUN_TIME	4




/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */
// Callback when the device has been started.  Callback event to 
// the ask for a battery check.
typedef void (*accelEnabler_t)(uint8_t event);

typedef struct
{
  accelEnabler_t     pfnAccelEnabler;  // Called when Enabler attribute changes
} monitorCBs_t;

/*********************************************************************
 * API FUNCTIONS 
 */

/*
 * Accel_AddService - Initializes the Accelerometer service by registering 
 *          GATT attributes with the GATT server. Only call this function once.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 */
extern bStatus_t monitor_AddService(uint32 services);

/*
 * Accel_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t monitor_RegisterAppCBs(monitorCBs_t *appCallbacks);


/*
 * Accel_SetParameter - Set an Accelerometer Profile parameter.
 *
 *    param - Profile parameter ID
 *    len   - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 */
extern bStatus_t monitor_SetParameter(uint16 param, uint8 len, void *value);
  
/*
 * Accel_GetParameter - Get an Accelerometer Profile parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 */
extern bStatus_t monitor_GetParameter(uint16 param, void *value);

/*********************************************************************
 * @fn          Accel_MeasNotify
 *
 * @brief       Send a notification containing a heart rate
 *              measurement.
 *
 * @param       connHandle - connection handle
 * @param       pNoti - pointer to notification structure
 *
 * @return      Success or Failure
 */
//extern bStatus_t monitor_MeasNotify(uint16 connHandle, attHandleValueNoti_t *pNoti);

//extern uint8 Accel_IsNotifyEnabled(void);
//extern void Accel_enableNotifyCfg(void);
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* MOTOR_MONIOR_H */
