/** ==================================================================================================================*\
  @file CanNm.c

 @brief CAN Network Management

 Implementation of CAN Network Management.

 @see ***.pdf
 \*====================================================================================================================*/

/*====================================================================================================================*\
    Załączenie nagłówków
 \*====================================================================================================================*/
#include "../includes/CanNm.h"
#include "../includes/NmStack_Types.h"
#include "../includes/fff.h"
#include <string.h>
/*====================================================================================================================*\
    Makra lokalne
 \*====================================================================================================================*/
#define CANNM_CBV_REPEAT_MESSAGE_REQUEST	0x01u  /**< [SWS_CanNm_00045] */
#define CANNM_CBV_ACTIVE_WAKEUP_BIT 		0x10
#define VENDOR_ID							0
#define MODULE_ID							0
/*====================================================================================================================*\
    Typy lokalne
 \*====================================================================================================================*/
typedef enum {
	CANNM_STATUS_INIT, CANNM_STATUS_UNINIT
} CanNm_InitStatusType;


/*====================================================================================================================*\
    Zmienne globalne
 \*====================================================================================================================*/
CanNm_InitStatusType InitStatus = CANNM_STATUS_UNINIT;

CanNm_InternalType CanNm_Internal = {
	.PduLength = 8,
	.TxMessageSdu = 0
};

static const CanNm_ConfigType* CanNm_ConfigPtr;
/*====================================================================================================================*\
    Zmienne lokalne (statyczne)
 \*====================================================================================================================*/

/*====================================================================================================================*\
    Deklaracje funkcji lokalnych
 \*====================================================================================================================*/
static inline PduLengthType CanNm_Internal_GetUserDataOffset( const CanNm_ConfigType* InConf );
static inline uint8* CanNm_Internal_GetUserDataPtr( const CanNm_ConfigType* InConf, uint8* MessageSduPtr );
static inline PduLengthType CanNm_Internal_GetUserDataLength( const CanNm_ConfigType* InputConf, const CanNm_InternalType* InternalConf );
/*====================================================================================================================*\
    Kod globalnych funkcji inline i makr funkcyjnych
 \*====================================================================================================================*/

/*====================================================================================================================*\
    Kod funkcji
 \*====================================================================================================================*/
//TODO mocki nie mogę być w kodzie źródłowym
#ifdef TEST
DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(Nm_NetworkMode, NetworkHandleType);
FAKE_VOID_FUNC(Nm_NetworkStartIndication, NetworkHandleType);
FAKE_VOID_FUNC(Nm_PrepareBusSleepMode, NetworkHandleType);	
FAKE_VOID_FUNC(Nm_BusSleepMode, NetworkHandleType);				
#endif

//TODO SWS, opisy funkcji
/** @brief CanNm_Init [SWS_CanNm_00208]
 *
 * Initialize the CanNm module.
 *
 */
/** must be called directly after canIf */ /** [SWS_CanNm_00253] */
void CanNm_Init(const CanNm_ConfigType* cannmConfigPtr) {

	CanNm_ConfigPtr = cannmConfigPtr;  /** [SWS_CanNm_00060] */
	CanNm_InternalType* ModuleInternal = &CanNm_Internal;

	ModuleInternal->Mode = NM_MODE_BUS_SLEEP;    /** [SWS_CanNm_00144] */
	ModuleInternal->State = NM_STATE_BUS_SLEEP;  /** [SWS_CanNm_00141] */
	ModuleInternal->Requested = FALSE;           /** [SWS_CanNm_00143] */ /*released*/
	ModuleInternal->TimeoutTimeLeft = 0;
	ModuleInternal->RepeatMessageTimeLeft = 0;
	ModuleInternal->MessageCycleTimeLeft = 0;
	ModuleInternal->WaitBusSleepTimeLeft = 0;
	ModuleInternal->VersionInfo.vendorID = VENDOR_ID;
	ModuleInternal->VersionInfo.moduleID = MODULE_ID;

//	CanNmBusLoadReductionEnabled = FALSE /** [SWS_CanNm_00023] */

	 if ((CanNm_ConfigPtr->CanNmChannelConfig->CanNmNodeIdEnabled) && (CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition != CANNM_PDU_OFF)) {
	 	CanNm_ConfigPtr->CanNmChannelConfig->CanNmTxPdu->TxPduRef->SduDataPtr[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition] = CanNm_ConfigPtr->CanNmChannelConfig->CanNmNodeId;/** [SWS_CanNm_00013 */
	}

// 	/** [SWS_CanNm_00085] */
	memset(ModuleInternal->TxMessageSdu, 0x00, 8);
	memset(ModuleInternal->RxMessageSdu, 0x00, 8);

// 	/** [SWS_CanNm_00025] */ //CanNmComUserDataSupport = TRUE
	uint8* destUserData = CanNm_Internal_GetUserDataPtr(CanNm_ConfigPtr, ModuleInternal->TxMessageSdu);
	uint8 userDataLength = CanNm_Internal_GetUserDataLength(CanNm_ConfigPtr, ModuleInternal);
	memset(destUserData, 0xFF, userDataLength);

// 	 /** [SWS_CanNm_00060] */

	InitStatus = CANNM_STATUS_INIT;
}

/**
 * @brief CanNm_DeInit [SWS_CanNm_91002]
 *
 * De-initializes the CanNm module.
 *
 TODO ustawić wskaźniki na NULL
 */
void CanNm_DeInit(void){

	if (InitStatus == CANNM_STATUS_INIT)	/** @req BSW232 */
	{
		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if (ModuleInternal->State != NM_STATE_BUS_SLEEP);

		else
		{
			ModuleInternal->State = NM_STATE_UNINIT;
			InitStatus = CANNM_STATUS_UNINIT;
		}
	}
	else
	{
		//Nothing to do
	}
}

/**
 * @brief CanNm_PassiveStartUp [SWS_CanNm_00211]
 *
 * Passive startup of the AUTOSAR CAN NM. It triggers the transition from Bus-Sleep Mode
 *  or Prepare Bus Sleep Mode to the Network Mode in Repeat Message State.
 *
 */
Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)	/**< [SWS_CanNm_00254] */
	{
		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if (ModuleInternal->Mode == NM_MODE_BUS_SLEEP || ModuleInternal->Mode == NM_MODE_PREPARE_BUS_SLEEP)
		{
			ModuleInternal->Mode = NM_MODE_NETWORK;		/**< [SWS_CanNm_00314] */ /**< [SWS_CanNm_00315] */ /**< [SWS_CanNm_00128] */
			ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;
			ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmTimeoutTime; 	/**< [SWS_CanNm_00096] */
			ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmRepeatMessageTime; 	/**< [SWS_CanNm_00102] */

			Nm_NetworkMode(nmChannelHandle);

			ret_val = E_OK;
		}
		else
		{
			ret_val = E_NOT_OK;		/**< [SWS_CanNm_00147] */
		}
	}
	else
	{
		ret_val = E_NOT_OK;
	}
	return ret_val;
}

/**
 * @brief CanNm_NetworkRequest [SWS_CanNm_00213]
 *
 * Request the network, since ECU needs to communicate on the bus.
 *
 */
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)	/**< [SWS_CanNm_00256] */
	{

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		ModuleInternal->Requested = TRUE;	/**< [SWS_CanNm_00255] */ /**< [SWS_CanNm_00104] */

		if(ModuleInternal->Mode == NM_MODE_BUS_SLEEP || ModuleInternal->Mode == NM_MODE_PREPARE_BUS_SLEEP)
		{
			ModuleInternal->Mode = NM_MODE_NETWORK;
			ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;
			ModuleInternal->TxMessageSdu[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition] |= CANNM_CBV_ACTIVE_WAKEUP_BIT;	/**< [SWS_CanNm_00401] */
			ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmTimeoutTime; 	/**< [SWS_CanNm_00096] */
			ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmRepeatMessageTime; 	/**< [SWS_CanNm_00102] */
			ret_val = E_OK;
		}
		else
		{
			ret_val = E_NOT_OK;
		}
	}
	else
	{
		 ret_val = E_NOT_OK;
	}
	return ret_val;
}


/**
 *  @brief CanNm_NetworkRelease [SWS_CanNm_00214]
 *
 *  Release the network, since ECU doesn't have to communicate on the bus.
 *
 */
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)	/**< [SWS_CanNm_00259] */
	{

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		ModuleInternal->Requested = FALSE; /**< [SWS_CanNm_00105] */

		if (ModuleInternal->Mode == NM_MODE_NETWORK)
		{
			if (ModuleInternal->State == NM_STATE_NORMAL_OPERATION)
			{
				ModuleInternal->Mode = NM_MODE_NETWORK;
				ModuleInternal->State = NM_STATE_READY_SLEEP; /**< [SWS_CanNm_00118] */
				ret_val = E_OK;
			}
			else
			{
				ret_val = E_NOT_OK;
			}
		}
		else
		{
			ret_val = E_NOT_OK;
		}
	}
	else
	{
		ret_val = E_NOT_OK;
	}
	return ret_val;
}

/**
 *  @brief CanNm_DisableCommunication [SWS_CanNm_00215]
 *
 *  Disable the NM PDU transmission ability due to a ISO14229 Communication Control (28hex) service
 *
 *  CANNM_COM_CONTROL_ENABLED = STD_OFF
 */

Std_ReturnType CanNm_DisableCommunication(NetworkHandleType nmChannelHandle);

/**
 * @brief CanNm_EnableCommunication [SWS_CanNm_00216]
 *
 * Enable the NM PDU transmission ability due to a ISO14229 Communication Control (28hex) service
 *
 * CANNM_COM_CONTROL_ENABLED = STD_OFF
 */
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle);

/**
 * @brief CanNm_SetUserData [SWS_CanNm_00217]
 *
 * Set user data for NM PDUs transmitted next on the bus.
 *
 * CANNM_USER_DATA_ENABLED == STD_ON
 */
Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle, const uint8* nmUserDataPtr)
{
	Std_ReturnType ret_val = E_OK;

	if (InitStatus == CANNM_STATUS_INIT)	/**< [SWS_CanNm_00265] */
	{

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		uint8* destUserData = CanNm_Internal_GetUserDataPtr(CanNm_ConfigPtr, ModuleInternal->TxMessageSdu);
		uint8 userDataLength = CanNm_Internal_GetUserDataLength(CanNm_ConfigPtr, ModuleInternal);

		memcpy(destUserData, nmUserDataPtr, userDataLength);	/**< [SWS_CanNm_00159] */
		ret_val = E_OK;

	}
	else
	{
		ret_val = E_NOT_OK;
	}
	return ret_val;
}

/**
 * @brief CanNm_GetUserData [SWS_CanNm_00218]
 *
 * Get user data out of the most recently received NM PDU.
 *
 * CANNM_USER_DATA_ENABLED == STD_ON
 */
Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle, uint8* nmUserDataPtr)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)	 /**< [SWS_CanNm_00267] */
	{

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		uint8* sourceUserData = CanNm_Internal_GetUserDataPtr(CanNm_ConfigPtr, ModuleInternal->RxMessageSdu);
		uint8 userDataLength = CanNm_Internal_GetUserDataLength(CanNm_ConfigPtr, ModuleInternal);

		memcpy(nmUserDataPtr, sourceUserData, userDataLength);	/**< [SWS_CanNm_00160] */

		ret_val = E_OK;
	}
	else
	{
		ret_val = E_NOT_OK;
	}
	return ret_val;
}

/**
 *  @brief CanNm_GetNodeIdentifier [SWS_CanNm_00219]
 *
 *  Get node identifier out of the most recently received NM PDU.
 *
 */
Std_ReturnType CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)	/**< [SWS_CanNm_00269] */
	{

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if (CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition == CANNM_PDU_OFF)
		{
			ret_val = E_NOT_OK;
		}
		else
		{
			*nmNodeIdPtr = ModuleInternal->RxMessageSdu[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition];	/**< [SWS_CanNm_00132] */
			ret_val = E_OK;
		}
	}
	else
	{
		ret_val = E_NOT_OK;
	}
	return ret_val;
}

/**
 * @brief CanNm_GetLocalNodeIdentifier [SWS_CanNm_00220]
 *
 * Get node identifier configured for the local node.
 *
 */
Std_ReturnType CanNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)	/**< [SWS_CanNm_00271] */
	{

		*nmNodeIdPtr = CanNm_ConfigPtr->CanNmChannelConfig->CanNmNodeId;	/**< [SWS_CanNm_00133] */

		ret_val = E_OK;
	}
	else
	{
		ret_val = E_NOT_OK;
	}
	return ret_val;
}

/**
 * @brief CanNm_RepeatMessageRequest [SWS_CanNm_00221]
 *
 * Set Repeat Message Request Bit for NM PDUs transmitted next on the bus.
 *
 */
Std_ReturnType CanNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) /**< [SWS_CanNm_00273] */
	{
		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if (ModuleInternal->State != NM_STATE_REPEAT_MESSAGE
				&& ModuleInternal->Mode != NM_MODE_PREPARE_BUS_SLEEP
				&& ModuleInternal->Mode != NM_MODE_BUS_SLEEP) /**< [SWS_CanNm_00137] */
		{
			if (ModuleInternal->State == NM_STATE_NORMAL_OPERATION || ModuleInternal->State == NM_STATE_READY_SLEEP)
			{
				ModuleInternal->Mode = NM_MODE_NETWORK;
				ModuleInternal->State = NM_STATE_REPEAT_MESSAGE; /**< [SWS_CanNm_00120] *//**< [SWS_CanNm_00112] */
				ModuleInternal->TxMessageSdu[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition] |= CANNM_CBV_REPEAT_MESSAGE_REQUEST; /**< [SWS_CanNm_00121] *//**< [SWS_CanNm_00113] */
				ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmTimeoutTime; 	/**< [SWS_CanNm_00096] */
				ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmRepeatMessageTime; 	/**< [SWS_CanNm_00102] */

				ret_val = E_OK;
			}
			else
			{
				ret_val = E_NOT_OK;
			}
		}
		else
		{
			ret_val = E_NOT_OK;
		}
	}
	else
	{
		ret_val = E_NOT_OK;
	}

	return ret_val;
}

/**
 * @brief CanNm_GetPduData [SWS_CanNm_00222]
 *
 * Get the whole PDU data out of the most recently received NM PDU.
 *
 */
Std_ReturnType CanNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)	/**< [SWS_CanNm_00275] */
	{

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		memcpy(nmPduDataPtr, ModuleInternal->RxMessageSdu, ModuleInternal->PduLength);  /**< [SWS_CanNm_00138] */

		ret_val = E_OK;
	}
	else
	{
		ret_val = E_NOT_OK;
	}
	return ret_val;

}

/**
 * @brief CanNm_GetState [SWS_CanNm_00223]
 *
 * Returns the state and the mode of the network management.
 *
 */
Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)	/**< [SWS_CanNm_00277] */
	{

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		*nmStatePtr = ModuleInternal->State;		/**< [SWS_CanNm_00091] */
		*nmModePtr = ModuleInternal->Mode;

		ret_val = E_OK;
	}
	else
	{
		ret_val = E_NOT_OK;
	}

	return ret_val;
}

/**
 * @brief CanNm_GetVersionInfo [SWS_CanNm_00224]
 *
 *This service returns the version information of this module
 *
 */
void CanNm_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	CanNm_InternalType* ModuleInternal = &CanNm_Internal;
	*versioninfo = ModuleInternal->VersionInfo;
}

/**
 * @brief CanNm_RequestBusSynchronization [SWS_CanNm_00226]
 *
 * Request bus synchronization.
 *
 * CanNmBusSynchronizationEnabled = FALSE; - req CANNM280
 */
Std_ReturnType CanNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle);


/**
 * @brief CanNm_CheckRemoteSleepIndication [SWS_CanNm_00227]
 *
 * Check if remote sleep indication takes place or not
 *
 * CanNmRemoteSleepIndEnabled = FALSE
 */
Std_ReturnType CanNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr);


/**
 * @brief CanNm_SetSleepReadyBit [SWS_CanNm_00338]
 *
 * Set the NM Coordinator Sleep Ready bit in the Control Bit Vector
 *
 *CanNmCoordinatorSyncSupport = FALSE
 */
Std_ReturnType CanNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit);


/**
 * @brief CanNm_TxConfirmation [SWS_CanNm_00228]
 *
 * The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PDU.
 *
 */
void CanNm_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
	if (InitStatus == CANNM_STATUS_INIT)	/**< [SWS_CanNm_00283] */
	{

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if ((ModuleInternal->Mode == NM_MODE_NETWORK) && (E_OK == result))
		{
			ModuleInternal->TimeoutTimeLeft =
					CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgTimeoutTime; /**< [SWS_CanNm_00099] */
		}
		else
		{
			//Nothing to do 
		}
	}
	else
	{
		//Nothing to do
	}
}

/**
 * @brief CanNm_RxIndication [SWS_CanNm_00231]
 *
 * Indication of a received PDU from a lower layer communication interface module
 *
 */
void CanNm_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{

	if (InitStatus == CANNM_STATUS_INIT) { /**< [SWS_CanNm_00277] */

		//CanNm_ConfigPtr
		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		memcpy(ModuleInternal->RxMessageSdu, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);  /**< [SWS_CanNm_00035] */

		boolean repeatMessageBitIndication = FALSE;
		if (CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition != CANNM_PDU_OFF)
		{
			uint8 cbv = ModuleInternal->RxMessageSdu[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition];
			repeatMessageBitIndication = cbv & CANNM_CBV_REPEAT_MESSAGE_REQUEST;
		}

		if (ModuleInternal->Mode == NM_MODE_BUS_SLEEP)
		{
			Nm_NetworkStartIndication(CanNm_ConfigPtr->CanNmChannelConfig->CanNmComMNetworkHandleRef);  /**< [SWS_CanNm_00127] */

		}
		else if (ModuleInternal->Mode == NM_MODE_PREPARE_BUS_SLEEP)
		{
			ModuleInternal->Mode = NM_MODE_NETWORK;		/**< [SWS_CanNm_00124] */
			ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;
			ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgTimeoutTime;  /**< [SWS_CanNm_00096] */
			ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmRepeatMessageTime;
			ModuleInternal->MessageCycleTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleTime - CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleOffset;  /**< [SWS_CanNm_00100] */
			
			Nm_NetworkMode(CanNm_ConfigPtr->CanNmChannelConfig->CanNmComMNetworkHandleRef);  /**< [SWS_CanNm_00097] */

		}
		else if (ModuleInternal->Mode == NM_MODE_NETWORK)
		{
			ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgTimeoutTime;  /**< [SWS_CanNm_00098] */
			if (repeatMessageBitIndication)
			{
				if (ModuleInternal->State == NM_STATE_READY_SLEEP)
				{
					ModuleInternal->Mode = NM_MODE_NETWORK;
					ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;		/**< [SWS_CanNm_00111] */
					ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmRepeatMessageTime;
					ModuleInternal->MessageCycleTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleTime - CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleOffset;  /**< [SWS_CanNm_00100] */
				}
				else if (ModuleInternal->State == NM_STATE_NORMAL_OPERATION)
				{
					ModuleInternal->Mode = NM_MODE_NETWORK;
					ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;		/**< [SWS_CanNm_00119] */
					ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmRepeatMessageTime;
					ModuleInternal->MessageCycleTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleTime - CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleOffset;  /**< [SWS_CanNm_00100] */
				}
				else
				{
					//Nothing to be done
				}
			}
		}
		else
		{
			//Nothing to be done
		}
	}
	else
	{
		//Nothing to be done	
	}
}

/**
 * @brief CanNm_ConfirmPnAvailability [SWS_CanNm_00344]
 *
 * Enables the PN filter functionality on the indicated NM channel.
 * Availability: The API is only available if CanNmGlobalPnSupport is TRUE.
 *
 * CanNmGlobalPnSupport = OFF
 */
void CanNm_ConfirmPnAvailability(NetworkHandleType nmChannelHandle);

/**
 * @brief CanNm_TriggerTransmit [SWS_CanNm_91001]
 *
 * Within this API, the upper layer module (called module) shall check whether the available
 * data fits into the buffer size reported by PduInfoPtr->SduLength. If it fits, it shall copy
 * its data into the buffer provided by PduInfoPtr->SduDataPtr and update the length of the actual
 * copied data in PduInfoPtr->SduLength. If not, it returns E_NOT_OK without changing PduInfoPtr.
 */
Std_ReturnType CanNm_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
{
	Std_ReturnType ret_val = E_NOT_OK;
	CanNm_InternalType* ModuleInternal = &CanNm_Internal;

	if(sizeof(ModuleInternal->TxMessageSdu) == PduInfoPtr->SduLength)
	{
		memcpy(ModuleInternal->TxMessageSdu, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
		ret_val = E_OK;
	}
	else
		ret_val = E_NOT_OK;

	return ret_val;
}

/**
 * @brief CanNm_MainFunction [SWS_CanNm_00234]
 *
 * Main function of the CanNm which processes the algorithm describes in that document
 *
 */
void CanNm_MainFunction(void){
	if (InitStatus == CANNM_STATUS_INIT)
	{
		CanNm_InternalType* ModuleInternal = &CanNm_Internal;
		if (ModuleInternal->Mode == NM_MODE_NETWORK)
		{
			if (CanNm_ConfigPtr->CanNmMainFunctionPeriod >= ModuleInternal->TimeoutTimeLeft)
			{
				ModuleInternal->TimeoutTimeLeft = 0;
				if (ModuleInternal->State == NM_STATE_REPEAT_MESSAGE)
				{
					ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmTimeoutTime; /**< [SWS_CanNm_00101]*/
				}
				else if (ModuleInternal->State == NM_STATE_NORMAL_OPERATION)
				{
					ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmTimeoutTime; /**< [SWS_CanNm_00117]*/
				}
				else if (ModuleInternal->State == NM_STATE_READY_SLEEP)
				{
					ModuleInternal->Mode = NM_MODE_PREPARE_BUS_SLEEP; /**< [SWS_CanNm_00109] */
					ModuleInternal->State = NM_STATE_PREPARE_BUS_SLEEP;
					ModuleInternal->WaitBusSleepTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmWaitBusSleepTime;
					// Notify 'Prepare Bus-Sleep Mode'
					Nm_PrepareBusSleepMode(CanNm_ConfigPtr->CanNmChannelConfig->CanNmComMNetworkHandleRef); /**< [SWS_CanNm_00114] */
				}
				else
				{
					//Nothing to be done
				}
			}
			else
			{
				ModuleInternal->TimeoutTimeLeft -= CanNm_ConfigPtr->CanNmMainFunctionPeriod;
			}
			
			if ((ModuleInternal->State == NM_STATE_REPEAT_MESSAGE) || (ModuleInternal->State == NM_STATE_NORMAL_OPERATION))
			{
				if (CanNm_ConfigPtr->CanNmMainFunctionPeriod >= ModuleInternal->MessageCycleTimeLeft)
				{
					ModuleInternal->MessageCycleTimeLeft = 0;/** [SWS_CanNm_00051] */
					ModuleInternal->MessageCycleTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleTime;  /**< [SWS_CanNm_00040] */
					}
				else
				{
					ModuleInternal->MessageCycleTimeLeft -= CanNm_ConfigPtr->CanNmMainFunctionPeriod;
				}
			}
			else
			{
				//Nothing to be done
			}
			
			if (ModuleInternal->State == NM_STATE_REPEAT_MESSAGE)
			{
				if (CanNm_ConfigPtr->CanNmMainFunctionPeriod >= ModuleInternal->RepeatMessageTimeLeft)
				{
					ModuleInternal->RepeatMessageTimeLeft = 0; /** [SWS_CanNm_00101]  */
					if (ModuleInternal->Requested)
					{
						ModuleInternal->Mode = NM_MODE_NETWORK;
						ModuleInternal->State = NM_STATE_NORMAL_OPERATION; /**< [SWS_CanNm_00103] */
						//CANNM_NODE_DETECTION_ENABLED == STD_ON
						if (CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition != CANNM_PDU_OFF)
						{
							ModuleInternal->TxMessageSdu[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition] &= 0xFF & (!CANNM_CBV_REPEAT_MESSAGE_REQUEST);  /** [SWS_CanNm_00107] */
						}
						else
						{
							//Nothing to be done
						}
					}
					else
					{
						ModuleInternal->Mode = NM_MODE_NETWORK;
						ModuleInternal->State = NM_STATE_READY_SLEEP;
						//CANNM_NODE_DETECTION_ENABLED == STD_ON
						if (CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition != CANNM_PDU_OFF)
						{
							ModuleInternal->TxMessageSdu[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition] &= 0xFF & (!CANNM_CBV_REPEAT_MESSAGE_REQUEST);  /** [SWS_CanNm_00107] */
						}
						else
						{
							//Nothing to be done
						}
					}
				}
				else
				{
					ModuleInternal->RepeatMessageTimeLeft -= CanNm_ConfigPtr->CanNmMainFunctionPeriod;
				}
			}
			else
			{
				//Nothing to be done
			}
		}
		else if (ModuleInternal->Mode == NM_MODE_PREPARE_BUS_SLEEP)
		{
			if (CanNm_ConfigPtr->CanNmMainFunctionPeriod >= ModuleInternal->WaitBusSleepTimeLeft) /**< [SWS_CanNm_00115] */
			{
				ModuleInternal->WaitBusSleepTimeLeft = 0;
				ModuleInternal->Mode = NM_MODE_BUS_SLEEP;
				ModuleInternal->State = NM_STATE_BUS_SLEEP;
				// Notify 'Bus-Sleep Mode'
				Nm_BusSleepMode(CanNm_ConfigPtr->CanNmChannelConfig->CanNmComMNetworkHandleRef);  /**< [SWS_CanNm_00126] */
			}
			else
			{
				ModuleInternal->WaitBusSleepTimeLeft -= CanNm_ConfigPtr->CanNmMainFunctionPeriod;
			}
		}
		else
		{
			//Nothing to be done
		}
	}
	else
	{
		//Nothing to be done
	}
}



/**
 * @brief Get user data offset byte index in NmPdu
 *
 */
static inline PduLengthType CanNm_Internal_GetUserDataOffset( const CanNm_ConfigType* InConf ) {
	PduLengthType userDataPos = 0;
    userDataPos += (InConf->CanNmChannelConfig->CanNmPduNidPosition == CANNM_PDU_OFF) ? 0 : 1;
    userDataPos += (InConf->CanNmChannelConfig->CanNmPduCbvPosition == CANNM_PDU_OFF) ? 0 : 1;
    return userDataPos;
}

/**
 * @brief Get the pointer to user data location in NmPdu
 *
 */
static inline uint8* CanNm_Internal_GetUserDataPtr( const CanNm_ConfigType* InConf, uint8* MessageSduPtr ) {
    uint8 userDataOffset = CanNm_Internal_GetUserDataOffset(InConf);
    return &MessageSduPtr[userDataOffset];
}

/**
 * @brief Get user data length
 *
 */
static inline PduLengthType CanNm_Internal_GetUserDataLength( const CanNm_ConfigType* InputConf, const CanNm_InternalType* InternalConf ) {
	PduLengthType userDataOffset = CanNm_Internal_GetUserDataOffset(InputConf);
    return InternalConf->PduLength - userDataOffset;
}

