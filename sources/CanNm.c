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
#define CANNM_CBV_REPEAT_MESSAGE_REQUEST	0x01u  /**< @req CANNM045 */
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
/**
 @brief Init


 */
/** must be called directly after canIf */ /** @req CANNM253 */
void CanNm_Init(const CanNm_ConfigType* cannmConfigPtr) {

	CanNm_ConfigPtr = cannmConfigPtr;  /** @req CANNM060 */
	CanNm_InternalType* ModuleInternal = &CanNm_Internal;

	ModuleInternal->Mode = NM_MODE_BUS_SLEEP;    /** @req CANNM144 */
	ModuleInternal->State = NM_STATE_BUS_SLEEP;  /** @req CANNM141 */
	ModuleInternal->Requested = FALSE;           /** @req CANNM143 */ /*released*/
	ModuleInternal->TimeoutTimeLeft = 0;
	ModuleInternal->RepeatMessageTimeLeft = 0;
	ModuleInternal->MessageCycleTimeLeft = 0;
	ModuleInternal->WaitBusSleepTimeLeft = 0;
	ModuleInternal->VersionInfo.vendorID = VENDOR_ID;
	ModuleInternal->VersionInfo.moduleID = MODULE_ID;

//	CanNmBusLoadReductionEnabled = FALSE /** @req CANNM023 */

	 if ((CanNm_ConfigPtr->CanNmChannelConfig->CanNmNodeIdEnabled) && (CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition != CANNM_PDU_OFF)) {
	 	CanNm_ConfigPtr->CanNmChannelConfig->CanNmTxPdu->TxPduRef->SduDataPtr[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition] = CanNm_ConfigPtr->CanNmChannelConfig->CanNmNodeId;/** @req CANNM013 */
	}

// 	/** @req CANNM085 */
	memset(ModuleInternal->TxMessageSdu, 0x00, 8);
	memset(ModuleInternal->RxMessageSdu, 0x00, 8);

// 	/** @req CANNM025 */ //CanNmComUserDataSupport = TRUE
	uint8* destUserData = CanNm_Internal_GetUserDataPtr(CanNm_ConfigPtr, ModuleInternal->TxMessageSdu);
	uint8 userDataLength = CanNm_Internal_GetUserDataLength(CanNm_ConfigPtr, ModuleInternal);
	memset(destUserData, 0xFF, userDataLength);

// 	 /** @req CANNM060 */

	InitStatus = CANNM_STATUS_INIT;
}

/**
 @brief DeInit
TODO ustawić wskaźniki na NULL
 */
void CanNm_DeInit(void){

	if (InitStatus == CANNM_STATUS_INIT)
	{		/** @req BSW232 */
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
 @brief CanNm_PassiveStartUp


 */
Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)
	{	/**< @req CANNM254 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if (ModuleInternal->Mode == NM_MODE_BUS_SLEEP || ModuleInternal->Mode == NM_MODE_PREPARE_BUS_SLEEP)
		{

			ModuleInternal->Mode = NM_MODE_NETWORK;		/**< @req CANNM314 */ /**< @req CANNM315 */ /**< @req CANNM128 */
			ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;
			ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmTimeoutTime; 	/**< @req CANNM096 */
			ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmRepeatMessageTime; 	/**< @req CANNM102 */

			Nm_NetworkMode(nmChannelHandle);

			ret_val = E_OK;
		}
		else
		{
			ret_val = E_NOT_OK;		/**< @req CANNM147 */
		}
	}
	else
	{
		ret_val = E_NOT_OK;
	}
	return ret_val;
}

/**
 @brief CanNm_NetworkRequest

 */
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)
	{	/**< @req CANNM256 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		ModuleInternal->Requested = TRUE;	/**< @req CANNM255 */ /**< @req CANNM104 */

		if(ModuleInternal->Mode == NM_MODE_BUS_SLEEP || ModuleInternal->Mode == NM_MODE_PREPARE_BUS_SLEEP)
		{
			ModuleInternal->Mode = NM_MODE_NETWORK;
			ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;
			ModuleInternal->TxMessageSdu[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition] |= CANNM_CBV_ACTIVE_WAKEUP_BIT;	/**< @req CANNM401 */
			ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmTimeoutTime; 	/**< @req CANNM096 */
			ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmRepeatMessageTime; 	/**< @req CANNM102 */
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
 @brief CanNm_NetworkRelease


 */
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)
	{ /**< @req CANNM259 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		ModuleInternal->Requested = FALSE; /**< @req CANNM105 */

		if (ModuleInternal->Mode == NM_MODE_NETWORK)
		{
			if (ModuleInternal->State == NM_STATE_NORMAL_OPERATION)
			{
				ModuleInternal->Mode = NM_MODE_NETWORK;
				ModuleInternal->State = NM_STATE_READY_SLEEP; /**< @req CANNM118 */
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
 @brief CanNm_DisableCommunication
CANNM_COM_CONTROL_ENABLED = STD_OFF

 */

Std_ReturnType CanNm_DisableCommunication(NetworkHandleType nmChannelHandle);

/**
 @brief CanNm_EnableCommunication
CANNM_COM_CONTROL_ENABLED = STD_OFF

 */
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle);

/**
 @brief CanNm_SetUserData
CANNM_USER_DATA_ENABLED == STD_ON

 */
Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle, const uint8* nmUserDataPtr)
{
	Std_ReturnType ret_val = E_OK;

	if (InitStatus == CANNM_STATUS_INIT)
	{ /**< @req CANNM265 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		uint8* destUserData = CanNm_Internal_GetUserDataPtr(CanNm_ConfigPtr, ModuleInternal->TxMessageSdu);
		uint8 userDataLength = CanNm_Internal_GetUserDataLength(CanNm_ConfigPtr, ModuleInternal);

		memcpy(destUserData, nmUserDataPtr, userDataLength);	/**< @req CANNM159 */
		ret_val = E_OK;

	}
	else
	{
		ret_val = E_NOT_OK;
	}
	return ret_val;
}

/**
 @brief CanNm_GetUserData
CANNM_USER_DATA_ENABLED == STD_ON

 */
Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle, uint8* nmUserDataPtr)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)
	{ /**< @req CANNM267 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		uint8* sourceUserData = CanNm_Internal_GetUserDataPtr(CanNm_ConfigPtr, ModuleInternal->RxMessageSdu);
		uint8 userDataLength = CanNm_Internal_GetUserDataLength(CanNm_ConfigPtr, ModuleInternal);

		memcpy(nmUserDataPtr, sourceUserData, userDataLength);	/**< @req CANNM160 */

		ret_val = E_OK;
	}
	else
	{
		ret_val = E_NOT_OK;
	}
	return ret_val;
}

/**
 @brief CanNm_GetNodeIdentifier


 */
Std_ReturnType CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)
	{ /**< @req CANNM269 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if (CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition == CANNM_PDU_OFF)
		{
			ret_val = E_NOT_OK;
		}
		else
		{
			*nmNodeIdPtr = ModuleInternal->RxMessageSdu[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition];	/**< @req CANNM132 */
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
 @brief CanNm_GetLocalNodeIdentifier


 */
Std_ReturnType CanNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)
	{ /**< @req CANNM271 */

		*nmNodeIdPtr = CanNm_ConfigPtr->CanNmChannelConfig->CanNmNodeId;	/**< @req CANNM133 */

		ret_val = E_OK;
	}
	else
	{
		ret_val = E_NOT_OK;
	}
	return ret_val;
}

/**
 @brief CanNm_RepeatMessageRequest


 */
Std_ReturnType CanNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) /**< @req CANNM273 */
	{
		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if (ModuleInternal->State != NM_STATE_REPEAT_MESSAGE
				&& ModuleInternal->Mode != NM_MODE_PREPARE_BUS_SLEEP
				&& ModuleInternal->Mode != NM_MODE_BUS_SLEEP) /**< @req CANNM137 */
		{
			if (ModuleInternal->State == NM_STATE_NORMAL_OPERATION || ModuleInternal->State == NM_STATE_READY_SLEEP)
			{
				ModuleInternal->Mode = NM_MODE_NETWORK;
				ModuleInternal->State = NM_STATE_REPEAT_MESSAGE; /**< @req CANNM120 *//**< @req CANNM112 */
				ModuleInternal->TxMessageSdu[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition] |= CANNM_CBV_REPEAT_MESSAGE_REQUEST; /**< @req CANNM121 *//**< @req CANNM113 */
				ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmTimeoutTime; 	/**< @req CANNM096 */
				ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmRepeatMessageTime; 	/**< @req CANNM102 */

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
 @brief CanNm_GetPduData

 */
Std_ReturnType CanNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)
	{ /**< @req CANNM275 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		memcpy(nmPduDataPtr, ModuleInternal->RxMessageSdu, ModuleInternal->PduLength);  /**< @req CANNM138 */

		ret_val = E_OK;
	}
	else
	{
		ret_val = E_NOT_OK;
	}
	return ret_val;

}

/**
 @brief CanNm_GetState


 */
Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr)
{
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT)
	{ /**< @req CANNM277 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		*nmStatePtr = ModuleInternal->State;		/**< @req CANNM091 */
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
 @brief CanNm_GetVersionInfo

@req CANNM224
 */
void CanNm_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	CanNm_InternalType* ModuleInternal = &CanNm_Internal;
	*versioninfo = ModuleInternal->VersionInfo;
}

/**
 @brief CanNm_RequestBusSynchronization
CanNmBusSynchronizationEnabled = FALSE; - req CANNM280

 */
Std_ReturnType CanNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle);

/**
 @brief CanNm_CheckRemoteSleepIndication
CanNmRemoteSleepIndEnabled = FALSE

 */
Std_ReturnType CanNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr);
/**
 @brief CanNm_SetSleepReadyBit
CanNmCoordinatorSyncSupport = FALSE

 */
Std_ReturnType CanNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit);


/**
 @brief CanNm_TxConfirmation

 */
void CanNm_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
	if (InitStatus == CANNM_STATUS_INIT)
	{ /**< @req CANNM283 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if ((ModuleInternal->Mode == NM_MODE_NETWORK) && (E_OK == result))
		{
			ModuleInternal->TimeoutTimeLeft =
					CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgTimeoutTime; /**< @req CANNM099 */
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
 @brief


 */
void CanNm_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM277 */

		//CanNm_ConfigPtr
		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		memcpy(ModuleInternal->RxMessageSdu, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);  /**< @req CANNM035 */

		boolean repeatMessageBitIndication = FALSE;
		if (CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition != CANNM_PDU_OFF)
		{
			uint8 cbv = ModuleInternal->RxMessageSdu[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition];
			repeatMessageBitIndication = cbv & CANNM_CBV_REPEAT_MESSAGE_REQUEST;
		}

		if (ModuleInternal->Mode == NM_MODE_BUS_SLEEP)
		{
			Nm_NetworkStartIndication(CanNm_ConfigPtr->CanNmChannelConfig->CanNmComMNetworkHandleRef);  /**< @req CANNM127 */

		}
		else if (ModuleInternal->Mode == NM_MODE_PREPARE_BUS_SLEEP)
		{
			ModuleInternal->Mode = NM_MODE_NETWORK;		/**< @req CANNM124 */
			ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;
			ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgTimeoutTime;  /**< @req CANNM096 */
			ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmRepeatMessageTime;
			ModuleInternal->MessageCycleTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleTime - CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleOffset;  /**< @req CANNM100 */
			
			Nm_NetworkMode(CanNm_ConfigPtr->CanNmChannelConfig->CanNmComMNetworkHandleRef);  /**< @req CANNM097 */

		}
		else if (ModuleInternal->Mode == NM_MODE_NETWORK)
		{
			ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgTimeoutTime;  /**< @req CANNM098 */
			if (repeatMessageBitIndication)
			{
				if (ModuleInternal->State == NM_STATE_READY_SLEEP)
				{
					ModuleInternal->Mode = NM_MODE_NETWORK;
					ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;		/**< @req CANNM111 */
					ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmRepeatMessageTime;
					ModuleInternal->MessageCycleTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleTime - CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleOffset;  /**< @req CANNM100 */
				}
				else if (ModuleInternal->State == NM_STATE_NORMAL_OPERATION)
				{
					ModuleInternal->Mode = NM_MODE_NETWORK;
					ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;		/**< @req CANNM119 */
					ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmRepeatMessageTime;
					ModuleInternal->MessageCycleTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleTime - CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleOffset;  /**< @req CANNM100 */
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
 @brief
CanNmGlobalPnSupport = OFF

 */
void CanNm_ConfirmPnAvailability(NetworkHandleType nmChannelHandle);

/**
 @brief CanNm_TriggerTransmit

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
 @brief


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
					ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmTimeoutTime; /**< @req CANNM101*/
				}
				else if (ModuleInternal->State == NM_STATE_NORMAL_OPERATION)
				{
					ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmTimeoutTime; /**< @req CANNM117*/
				}
				else if (ModuleInternal->State == NM_STATE_READY_SLEEP)
				{
					ModuleInternal->Mode = NM_MODE_PREPARE_BUS_SLEEP; /**< @req CANNM109 */
					ModuleInternal->State = NM_STATE_PREPARE_BUS_SLEEP;
					ModuleInternal->WaitBusSleepTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmWaitBusSleepTime;
					// Notify 'Prepare Bus-Sleep Mode'
					Nm_PrepareBusSleepMode(CanNm_ConfigPtr->CanNmChannelConfig->CanNmComMNetworkHandleRef); /**< @req CANNM114 */
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
					ModuleInternal->MessageCycleTimeLeft = 0;/** @req CANNM051  */
					ModuleInternal->MessageCycleTimeLeft = CanNm_ConfigPtr->CanNmChannelConfig->CanNmMsgCycleTime;  /**< @req CANNM040 */
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
					ModuleInternal->RepeatMessageTimeLeft = 0; /** @req CANNM101  */
					if (ModuleInternal->Requested)
					{
						ModuleInternal->Mode = NM_MODE_NETWORK;
						ModuleInternal->State = NM_STATE_NORMAL_OPERATION; /**< @req CANNM103 */
						//CANNM_NODE_DETECTION_ENABLED == STD_ON
						if (CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition != CANNM_PDU_OFF)
						{
							ModuleInternal->TxMessageSdu[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition] &= 0xFF & (!CANNM_CBV_REPEAT_MESSAGE_REQUEST);  /** @req CANNM107 */
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
							ModuleInternal->TxMessageSdu[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduCbvPosition] &= 0xFF & (!CANNM_CBV_REPEAT_MESSAGE_REQUEST);  /** @req CANNM107 */
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
			if (CanNm_ConfigPtr->CanNmMainFunctionPeriod >= ModuleInternal->WaitBusSleepTimeLeft) /**< @req CANNM115 */
			{
				ModuleInternal->WaitBusSleepTimeLeft = 0;
				ModuleInternal->Mode = NM_MODE_BUS_SLEEP;
				ModuleInternal->State = NM_STATE_BUS_SLEEP;
				// Notify 'Bus-Sleep Mode'
				Nm_BusSleepMode(CanNm_ConfigPtr->CanNmChannelConfig->CanNmComMNetworkHandleRef);  /**< @req CANNM126 */
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
 * @brief get the pointer to user data location in NmPdu
 *
 */
static inline uint8* CanNm_Internal_GetUserDataPtr( const CanNm_ConfigType* InConf, uint8* MessageSduPtr ) {
    uint8 userDataOffset = CanNm_Internal_GetUserDataOffset(InConf);
    return &MessageSduPtr[userDataOffset];
}

/**
 * @brief get user data length
 *
 */
static inline PduLengthType CanNm_Internal_GetUserDataLength( const CanNm_ConfigType* InputConf, const CanNm_InternalType* InternalConf ) {
	PduLengthType userDataOffset = CanNm_Internal_GetUserDataOffset(InputConf);
    return InternalConf->PduLength - userDataOffset;
}

