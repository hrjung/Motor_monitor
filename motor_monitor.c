/******************************************************************************

 @file  motor monitor.c

 @brief Accelerometer Profile for CC26xx.

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

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"

#include "motor_monitor.h"
#include <ti/mw/display/Display.h>

extern Display_Handle dispHandle;

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
#define SERVAPP_NUM_ATTR_SUPPORTED        13
//#define SERVAPP_NUM_ATTR_SUPPORTED        17

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Monitor Service UUID
CONST uint8 MonitorServUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(MONITOR_SERVICE_UUID), HI_UINT16(MONITOR_SERVICE_UUID)
};

// Monitor Freq UUID : 0xFFE2
CONST uint8 mntrFreqUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(MONITOR_FREQ_UUID), HI_UINT16(MONITOR_FREQ_UUID)
};

// Monitor Speed UUID : 0xFFE3
CONST uint8 mntrRpmUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(MONITOR_RPM_UUID), HI_UINT16(MONITOR_RPM_UUID)
};

CONST uint8 mntrVoltageUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(MONITOR_VOLTAGE_UUID), HI_UINT16(MONITOR_VOLTAGE_UUID)
};

CONST uint8 mntrRunTimeUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(MONITOR_RUN_TIME_UUID), HI_UINT16(MONITOR_RUN_TIME_UUID)
};

// Accelerometer noti UUID : 0xFFA5
//CONST uint8 nofiCfgUUID[ATT_BT_UUID_SIZE] =
//{
//  LO_UINT16(ACCEL_NOTI_CFG_UUID), HI_UINT16(ACCEL_NOTI_CFG_UUID)
//};

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static monitorCBs_t *monitor_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// Accelerometer Service attribute
static CONST gattAttrType_t MonitorService = { ATT_BT_UUID_SIZE, MonitorServUUID };

// Characteristic Properties
static uint8 MntrFreqCharProps = GATT_PROP_READ;
static uint8 MntrRpmCharProps = GATT_PROP_READ;
static uint8 MntrVoltageCharProps = GATT_PROP_READ;
static uint8 MntrRunTimeCharProps = GATT_PROP_READ;

//static uint8 accelNofiCfg = 0; //disable
// Characteristics value, 3 valuse for x, y, z
static uint32 MntrFreqValue = 0;
static uint32 MntrRpmValue = 0;
static uint32 MntrVoltageValue = 0;
static uint32 MntrRunTimeValue = 0;

// Characteristic Configs for notify
static gattCharCfg_t *MntrFreqConfig;
static gattCharCfg_t *MntrRpmConfig;
static gattCharCfg_t *MntrVoltageConfig;
static gattCharCfg_t *MntrRunTimeConfig;
//static gattCharCfg_t *accelSensorNofiConfig;

// Characteristic user descriptions
static uint8 MntrFreqUserDesc[20] 			= "Motor Frequency    ";
static uint8 MntrSpeedRpmCharUserDesc[20] 	= "Motor Speed        ";
static uint8 MntrDcVoltageCharUserDesc[20] 	= "Motor DC voltage   ";
static uint8 MntrRunTimeCharUserDesc[20] 	= "Motor Run Time     ";

/*********************************************************************
 * Profile Attributes - Table
 */
static gattAttribute_t mntrAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] =
{
  // Accelerometer Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&MonitorService                  /* pValue */
  },

  	 //===============================================
	// motor frequency Characteristic Declaration
	{
		  { ATT_BT_UUID_SIZE, characterUUID }, //type 0x2803
		  GATT_PERMIT_READ,						// permissions
		  0,									// handle
		  &MntrFreqCharProps				// pValue
	},

	  // motor frequency Characteristic Value
	  {
		{ ATT_BT_UUID_SIZE, mntrFreqUUID },
		GATT_PERMIT_READ,
		0,
		(uint8 *)&MntrFreqValue
	  },
#if 0
      // motor frequency configuration
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&MntrFreqConfig
      },
#endif
	  // Sensor Min Characteristic User Description
	  {
		{ ATT_BT_UUID_SIZE, charUserDescUUID },
		GATT_PERMIT_READ,
		0,
		MntrFreqUserDesc
	  },

	  //===============================================
		// Accelero sensor calc Characteristic Declaration
		{
		  { ATT_BT_UUID_SIZE, characterUUID }, //type 0x2803
		  GATT_PERMIT_READ,						// permissions
		  0,									// handle
		  &MntrRpmCharProps				// pValue
		},

		  // sensor calc Characteristic Value
		  {
			{ ATT_BT_UUID_SIZE, mntrRpmUUID },
			GATT_PERMIT_READ,
			0,
			(uint8 *)&MntrRpmValue
		  },
#if 0
	      // sensor calc configuration
	      {
	        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
	        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
	        0,
	        (uint8 *)&MntrRpmConfig
	      },
#endif
		  // sensor calc Characteristic User Description
		  {
			{ ATT_BT_UUID_SIZE, charUserDescUUID },
			GATT_PERMIT_READ,
			0,
			MntrSpeedRpmCharUserDesc
		  },
		  //===============================================
			// Accelero sensor calc Characteristic Declaration
			{
			  { ATT_BT_UUID_SIZE, characterUUID }, //type 0x2803
			  GATT_PERMIT_READ,						// permissions
			  0,									// handle
			  &MntrVoltageCharProps			// pValue
			},

			  // sensor calc Characteristic Value
			  {
				{ ATT_BT_UUID_SIZE, mntrVoltageUUID },
				GATT_PERMIT_READ,
				0,
				(uint8 *)&MntrVoltageValue
			  },
#if 0
		      // sensor calc configuration
		      {
		        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
		        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
		        0,
		        (uint8 *)&MntrVoltageConfig
		      },
#endif
			  // sensor calc Characteristic User Description
			  {
				{ ATT_BT_UUID_SIZE, charUserDescUUID },
				GATT_PERMIT_READ,
				0,
				MntrDcVoltageCharUserDesc
			  },
		  //===============================================
			// Monitor Run time Characteristic Declaration
			{
			  { ATT_BT_UUID_SIZE, characterUUID }, //type 0x2803
			  GATT_PERMIT_READ,						// permissions
			  0,									// handle
			  &MntrRunTimeCharProps			// pValue
			},

		  // Run time Characteristic Value
		  {
			{ ATT_BT_UUID_SIZE, mntrRunTimeUUID },
			GATT_PERMIT_READ,
			0,
			(uint8 *)&MntrRunTimeValue
		  },
#if 0
		  // Run time configuration
		  {
			{ ATT_BT_UUID_SIZE, clientCharCfgUUID },
			GATT_PERMIT_READ | GATT_PERMIT_WRITE,
			0,
			(uint8 *)&MntrRunTimeConfig
		  },
#endif
		  // Run time Characteristic User Description
		  {
			{ ATT_BT_UUID_SIZE, charUserDescUUID },
			GATT_PERMIT_READ,
			0,
			MntrRunTimeCharUserDesc
		  },

//	    // Accel Notify config Characteristic Declaration
//	    {
//	       { ATT_BT_UUID_SIZE, characterUUID },
//	       GATT_PERMIT_READ,
//	       0,
//	       &accelSensorNotiCfgCharProps
//	    },
//
//	      // Accelerometer Enable Characteristic Value
//	      {
//	        { ATT_BT_UUID_SIZE, nofiCfgUUID },
//	        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
//	        0,
//	        &accelNofiCfg
//	      },
//
//	      // Accelerometer Enable User Description
//	      {
//	        { ATT_BT_UUID_SIZE, charUserDescUUID },
//	        GATT_PERMIT_READ,
//	        0,
//	        (uint8*)&accelSensorNotiCfgUserDesc
//	      },

};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t monitor_ReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint16_t *pLen,
                                  uint16_t offset, uint16_t maxLen,
                                  uint8_t method);

static bStatus_t monitor_WriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                   uint8_t *pValue, uint16_t len,
                                   uint16_t offset, uint8_t method);

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Accelerometer Service Callbacks
// Note: When an operation on a characteristic requires authorization and 
// pfnAuthorizeAttrCB is not defined for that characteristic's service, the 
// Stack will report a status of ATT_ERR_UNLIKELY to the client.  When an 
// operation on a characteristic requires authorization the Stack will call 
// pfnAuthorizeAttrCB to check a client's authorization prior to calling
// pfnReadAttrCB or pfnWriteAttrCB, so no checks for authorization need to be 
// made within these functions.
CONST gattServiceCBs_t  monitorCBs =
{
  monitor_ReadAttrCB,  // Read callback function pointer
  monitor_WriteAttrCB, // Write callback function pointer
  NULL               // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      Accel_AddService
 *
 * @brief   Initializes the Accelerometer service by
 *          registering GATT attributes with the GATT server. Only
 *          call this function once.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t monitor_AddService(uint32 services)
{
  uint8 status = SUCCESS;
  size_t allocSize = sizeof(gattCharCfg_t) * linkDBNumConns;

  // Allocate Client Characteristic Configuration tables
  MntrFreqConfig = (gattCharCfg_t *)ICall_malloc(allocSize);
  if (MntrFreqConfig == NULL)
  {
    return (bleMemAllocError);
  }

  MntrRpmConfig = (gattCharCfg_t *)ICall_malloc(allocSize);
  if (MntrRpmConfig == NULL)
  {
	ICall_free(MntrFreqConfig);
    return (bleMemAllocError);
  }

  MntrVoltageConfig = (gattCharCfg_t *)ICall_malloc(allocSize);
  if (MntrVoltageConfig == NULL)
  {
	ICall_free(MntrFreqConfig);
	ICall_free(MntrRpmConfig);
    return (bleMemAllocError);
  }

  MntrRunTimeConfig = (gattCharCfg_t *)ICall_malloc(allocSize);
  if (MntrRunTimeConfig == NULL)
  {
	ICall_free(MntrFreqConfig);
	ICall_free(MntrRpmConfig);
	ICall_free(MntrVoltageConfig);
    return (bleMemAllocError);
  }

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, MntrFreqConfig);
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, MntrRpmConfig);
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, MntrVoltageConfig);
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, MntrRunTimeConfig);

  if (services & MONITOR_SERVICE)
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService(mntrAttrTbl,
                                         GATT_NUM_ATTRS(mntrAttrTbl),
                                         GATT_MAX_ENCRYPT_KEY_SIZE,
                                         &monitorCBs);
  }

  return (status);
}

/*********************************************************************
 * @fn      Accel_RegisterAppCBs
 *
 * @brief   Does the profile initialization.  Only call this function
 *          once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t monitor_RegisterAppCBs(monitorCBs_t *appCallbacks)
{
  if (appCallbacks)
  {
    monitor_AppCBs = appCallbacks;
    
    return (SUCCESS);
  }
  else
  {
    return (bleAlreadyInRequestedMode);
  }
}

/*********************************************************************
 * @fn      Accel_SetParameter
 *
 * @brief   Set an Accelerometer Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len   - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *                  the parameter ID and WILL be cast to the appropriate 
 *                  data type (example: data type of uint16 will be cast to 
 *                  uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t monitor_SetParameter(uint16 param, uint8 len, void *value)
{
  bStatus_t ret = SUCCESS;

  switch (param)
  {
    case MONITOR_FREQ:
      if (len == sizeof(uint32))
      {
    	  memcpy(&MntrFreqValue, value, len);
          // See if Notification has been enabled
          GATTServApp_ProcessCharCfg( MntrFreqConfig, (uint8 *)&MntrFreqValue, FALSE,
	  	  	  	  	 	 	 	 	  mntrAttrTbl, GATT_NUM_ATTRS( mntrAttrTbl ),
                                      INVALID_TASK_ID, monitor_ReadAttrCB );
      }
      else
        ret = bleInvalidRange;

      break;

    case MONITOR_RPM:
      if (len == sizeof(uint32))
      {
    	  memcpy(&MntrRpmValue, value, len);
          // See if Notification has been enabled
          GATTServApp_ProcessCharCfg( MntrRpmConfig, (uint8 *)&MntrRpmValue, FALSE,
	  	  	  	  	  	  	  	  	  mntrAttrTbl, GATT_NUM_ATTRS( mntrAttrTbl ),
                                      INVALID_TASK_ID, monitor_ReadAttrCB );
      }
      else
        ret = bleInvalidRange;

      break;

    case MONITOR_VOLTAGE:
      if (len == sizeof(uint32))
      {
    	  memcpy(&MntrVoltageValue, value, len);
          // See if Notification has been enabled
          GATTServApp_ProcessCharCfg( MntrVoltageConfig, (uint8 *)&MntrVoltageValue, FALSE,
	  	  	  	  	  	  	  	  	  mntrAttrTbl, GATT_NUM_ATTRS( mntrAttrTbl ),
                                      INVALID_TASK_ID, monitor_ReadAttrCB );
      }
      else
        ret = bleInvalidRange;

      break;

    case MONITOR_RUN_TIME:
      if (len == sizeof(uint32))
      {
    	  memcpy(&MntrRunTimeValue, value, len);
          // See if Notification has been enabled
          GATTServApp_ProcessCharCfg( MntrRunTimeConfig, (uint8 *)&MntrRunTimeValue, FALSE,
        		  	  	  	  	  	  mntrAttrTbl, GATT_NUM_ATTRS( mntrAttrTbl ),
                                      INVALID_TASK_ID, monitor_ReadAttrCB );
      }
      else
        ret = bleInvalidRange;

      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return (ret);
}

/*********************************************************************
 * @fn      Accel_GetParameter
 *
 * @brief   Get an Accelerometer Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *                  the parameter ID and WILL be cast to the appropriate 
 *                  data type (example: data type of uint16 will be cast to 
 *                  uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t monitor_GetParameter(uint16 param, void *value)
{
  bStatus_t ret = SUCCESS;
  switch (param)
  {

 //	case ACCEL_NOTIFY_CFG:
//		*((uint8*)value) = accelNofiCfg;
//		break;

    case MONITOR_FREQ:
      memcpy(value, &MntrFreqValue, sizeof(uint32));
      break;

    case MONITOR_RPM:
      memcpy(value, &MntrRpmValue, sizeof(uint32));
      break;

    case MONITOR_VOLTAGE:
      memcpy(value, &MntrVoltageValue, sizeof(uint32));
      break;

    case MONITOR_RUN_TIME:
      memcpy(value, &MntrRunTimeValue, sizeof(uint32));
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return (ret);
}

/*********************************************************************
 * @fn          accel_ReadAttr
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr      - pointer to attribute
 * @param       pValue     - pointer to data to be read
 * @param       pLen       - length of data to be read
 * @param       offset     - offset of the first octet to be read
 * @param       maxLen     - maximum length of data to be read
 * @param       method     - type of read message 
 *
 * @return      SUCCESS, blePending or Failure
 */
static bStatus_t monitor_ReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint16_t *pLen,
                                  uint16_t offset, uint16_t maxLen,
                                  uint8_t method)
{
  uint16 uuid;
  bStatus_t status = SUCCESS;

  // Make sure it's not a blob operation
  if (offset > 0)
  {
    return (ATT_ERR_ATTR_NOT_LONG);
  }

  Display_print0(dispHandle, 2, 0, "monitor_ReadAttrCB");
  if (pAttr->type.len == ATT_BT_UUID_SIZE)
  {    
    // 16-bit UUID
    uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch (uuid)
    {
      // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
      // gattserverapp handles those types for reads

      case MONITOR_FREQ_UUID:
        *pLen = sizeof(uint32);
        memcpy(pValue, pAttr->pValue, *pLen);
        //Display_print4(dispHandle, 2, 0, "accel_ReadAttrCB len=%d val=%d %d %d", *pLen, pValue[0],pValue[1],pValue[2]);
        break;

      case MONITOR_RPM_UUID:
        *pLen = sizeof(uint32);
        memcpy(pValue, pAttr->pValue, *pLen);
        //Display_print4(dispHandle, 2, 0, "accel_ReadAttrCB len=%d val=%d %d %d", *pLen, pValue[0],pValue[1],pValue[2]);
        break;

      case MONITOR_VOLTAGE_UUID:
        *pLen = sizeof(uint32);
        memcpy(pValue, pAttr->pValue, *pLen);
        //Display_print4(dispHandle, 2, 0, "accel_ReadAttrCB len=%d val=%d %d %d", *pLen, pValue[0],pValue[1],pValue[2]);
        break;

      case MONITOR_RUN_TIME_UUID:
        *pLen = sizeof(uint32);
        memcpy(pValue, pAttr->pValue, *pLen);
        //Display_print4(dispHandle, 2, 0, "accel_ReadAttrCB len=%d val=%d %d %d", *pLen, pValue[0],pValue[1],pValue[2]);
        break;

      default:
        // Should never get here!
        *pLen = 0;
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    *pLen = 0;
    status = ATT_ERR_INVALID_HANDLE;
  }
  
  return (status);
}

/*********************************************************************
 * @fn      accel_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr      - pointer to attribute
 * @param   pValue     - pointer to data to be written
 * @param   len        - length of data
 * @param   offset     - offset of the first octet to be written
 * @param   method     - type of write message 
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t monitor_WriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                   uint8_t *pValue, uint16_t len,
                                   uint16_t offset, uint8_t method)
{
	bStatus_t status = SUCCESS;
    uint16 uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);

    Display_print3(dispHandle, 2, 0, "monitor_WriteAttrCB len=%d %d uuid=0x%x", pAttr->type.len, len, uuid);
    if (offset > 0)
    {
      return (ATT_ERR_ATTR_NOT_LONG);
    }

    if ( pAttr->type.len == ATT_BT_UUID_SIZE )
    {

		switch (uuid)
		{
		  default:
			  // Should never get here!
			  status = ATT_ERR_ATTR_NOT_FOUND;
		}
    }
    else
    {
		// 128-bit UUID
		status = ATT_ERR_INVALID_HANDLE;
    }

  // If an attribute changed then callback function to notify application of 
  // change.
//  if ((notify != 0xFF) && accel_AppCBs && accel_AppCBs->pfnAccelEnabler)
//  {
//    accel_AppCBs->pfnAccelEnabler();
//  }
  
  return (status);
}
/*********************************************************************
*********************************************************************/
