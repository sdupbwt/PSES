/** ==================================================================================================================*\
  @file CanNm.c

 @brief CAN Network Management

 Implementation of CAN Network Management.

 @see ***.pdf
 \*====================================================================================================================*/

/*====================================================================================================================*\
    Załączenie nagłówków
 \*====================================================================================================================*/
#include "CanNm.h"
#include "NmStack_Types.h"
#include <string.h>
/*====================================================================================================================*\
    Makra lokalne
 \*====================================================================================================================*/
#define CANNM_CBV_REPEAT_MESSAGE_REQUEST	0x01u  /**< @req CANNM045 */
#define CANNM_CBV_ACTIVE_WAKEUP_BIT 		0x10
/*====================================================================================================================*\
    Typy lokalne
 \*====================================================================================================================*/
typedef enum {
	CANNM_STATUS_INIT, CANNM_STATUS_UNINIT
} CanNm_InitStatusType;

typedef struct {
	Nm_ModeType Mode; /**< @req CANNM092 */
	Nm_StateType State; /**< @req CANNM094 */
	boolean Requested;
    uint8 TxMessageSdu[8];
    uint8 RxMessageSdu[8];
    PduLengthType PduLength;
    Std_VersionInfoType VersionInfo;
    uint32 TimeoutTimeLeft;
    uint32 RepeatMessageTimeLeft;
    uint32 MessageCycleTimeLeft;
} CanNm_InternalType;

/*====================================================================================================================*\
    Zmienne globalne
 \*====================================================================================================================*/
CanNm_InitStatusType InitStatus = CANNM_STATUS_UNINIT;

CanNm_InternalType CanNm_Internal;

static const CanNm_ConfigType* CanNm_ConfigPtr;
/*====================================================================================================================*\
    Zmienne lokalne (statyczne)
 \*====================================================================================================================*/

/*====================================================================================================================*\
    Deklaracje funkcji lokalnych
 \*====================================================================================================================*/

/*====================================================================================================================*\
    Kod globalnych funkcji inline i makr funkcyjnych
 \*====================================================================================================================*/

/*====================================================================================================================*\
    Kod funkcji
 \*====================================================================================================================*/

/**
 @brief Init


 */
/** must be called directly after canIf /** @req CANNM253 */
void CanNm_Init(const CanNm_ConfigType* cannmConfigPtr) {

	CanNm_ConfigPtr = cannmConfigPtr;  /**< @req CANNM060 */
	CanNm_InternalType* ModuleInternal = &CanNm_Internal;

	ModuleInternal->Mode = NM_MODE_BUS_SLEEP;    /** @req CANNM144 */
	ModuleInternal->State = NM_STATE_BUS_SLEEP;  /** @req CANNM141 */
	ModuleInternal->Requested = FALSE;           /** @req CANNM143 */ /*released*/

//	CanNmBusLoadReductionActive = FALSE; /** @req CANNM023 */

//	CanNm_ConfigPtr

	/** @req CANNM085 */
	memset(ModuleInternal->TxMessageSdu, 0x00, 8);
	memset(ModuleInternal->RxMessageSdu, 0x00, 8);

	/** @req CANNM025 */ //TODO CanNmComUserDataSupport is enabled
	uint8* destUserData = CanNm_Internal_GetUserDataPtr(CanNm_ConfigPtr, ModuleInternal->TxMessageSdu);
	uint8 userDataLength = CanNm_Internal_GetUserDataLength(CanNm_ConfigPtr);
	memset(destUserData, 0xFF, userDataLength);

	 /** @req CANNM060 */

	InitStatus = CANNM_STATUS_INIT;
}

/**
 @brief DeInit

TODO to co on m�wi�
 */
void CanNm_DeInit(void){

	if (InitStatus == CANNM_STATUS_INIT) {		/** @req BSW232 */
		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		ModuleInternal->State = NM_STATE_UNINIT;
		InitStatus = CANNM_STATUS_UNINIT;
	}
	else {
	}
}

/**
 @brief CanNm_PassiveStartUp


 */
Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle){

	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) {	/**< @req CANNM254 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if (ModuleInternal->Mode == NM_MODE_BUS_SLEEP || ModuleInternal->Mode == NM_MODE_PREPARE_BUS_SLEEP) {

			ModuleInternal->Mode = NM_MODE_NETWORK;		/**< @req CANNM314 */ /**< @req CANNM315 */ /**< @req CANNM128 */
			ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;
			ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmTimeoutTime; 	/**< @req CANNM096 */
			ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmRepeatMessageTime; 	/**< @req CANNM102 */
//			ModuleInternal->MessageCycleTimeLeft = CanNm_ConfigPtr->MessageCycleOffsetTime;
			// Notify 'Network Mode'
			Nm_NetworkMode(nmChannelHandle); /**< @req CANNM097 */

			ret_val = E_OK;
		} else {
			ret_val = E_NOT_OK;		/**< @req CANNM147 */
		}

	} else {
		ret_val = E_NOT_OK;
	}

	return ret_val;
}

/**
 @brief CanNm_NetworkRequest
TODO E_OK - sprawdzic stasha

 */
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle){
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) {	/**< @req CANNM256 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		ModuleInternal->Requested = TRUE;	/**< @req CANNM255 */ /**< @req CANNM104 */

		if(ModuleInternal->Mode == NM_MODE_BUS_SLEEP || ModuleInternal->State == NM_MODE_PREPARE_BUS_SLEEP)
			ModuleInternal->Mode = NM_MODE_NETWORK;
			ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;
			ModuleInternal->TxMessageSdu[CanNm_ConfigPtr->CanNmPduCbvPosition] |= CANNM_CBV_ACTIVE_WAKEUP_BIT;	/**< @req CANNM401 */
	}else{
		 ret_val = E_NOT_OK;
	}
	return ret_val;
}


/**
 @brief CanNm_NetworkRelease


 */
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle){
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM259 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		ModuleInternal->Requested = FALSE; /**< @req CANNM105 */

		if (ModuleInternal->Mode == NM_MODE_NETWORK) {
			if (ModuleInternal->State == NM_STATE_NORMAL_OPERATION) {
				ModuleInternal->Mode = NM_MODE_NETWORK;
				ModuleInternal->State = NM_STATE_READY_SLEEP; /**< @req CANNM118 */
				ret_val = E_OK;
			} else {
				ret_val = E_NOT_OK;
			}
		} else {
			ret_val = E_NOT_OK;
		}
	} else {
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


 */
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle);

/**
 @brief CanNm_SetUserData
CANNM_USER_DATA_ENABLED == STD_ON

 */
Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle, const uint8* nmUserDataPtr){
	Std_ReturnType ret_val = E_OK;

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM265 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		uint8* destUserData = CanNm_Internal_GetUserDataPtr(CanNm_ConfigPtr, ModuleInternal->TxMessageSdu);
		uint8 userDataLength = CanNm_Internal_GetUserDataLength(CanNm_ConfigPtr);

		memcpy(destUserData, nmUserDataPtr, userDataLength);	/**< @req CANNM159 */
		ret_val = E_OK;

	} else {
		ret_val = E_NOT_OK;
	}

	return ret_val;
}

/**
 @brief CanNm_GetUserData
CANNM_USER_DATA_ENABLED == STD_ON

 */
Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle, uint8* nmUserDataPtr){
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM267 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		uint8* sourceUserData = CanNm_Internal_GetUserDataPtr(CanNm_ConfigPtr, ModuleInternal->RxMessageSdu);
		uint8 userDataLength = CanNm_Internal_GetUserDataLength(CanNm_ConfigPtr);

		memcpy(nmUserDataPtr, sourceUserData, userDataLength);	/**< @req CANNM160 */

		ret_val = E_OK;
	} else {
		ret_val = E_NOT_OK;
	}

	return ret_val;
}

/**
 @brief CanNm_Transmit


 */
Std_ReturnType CanNm_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr){

}

/**
 @brief CanNm_GetNodeIdentifier


 */
Std_ReturnType CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr){
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM269 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if (CanNm_ConfigPtr->CanNmPduNidPosition == CANNM_PDU_OFF) {
			ret_val = E_NOT_OK;
		} else {
			*nmNodeIdPtr = ModuleInternal->RxMessageSdu[CanNm_ConfigPtr->CanNmPduNidPosition];	/**< @req CANNM132 */
			ret_val = E_OK;
		}

		ret_val = E_OK;
	} else {
		ret_val = E_NOT_OK;
	}

	return ret_val;
}

/**
 @brief CanNm_GetLocalNodeIdentifier


 */
Std_ReturnType CanNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr){
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM271 */

		*nmNodeIdPtr = CanNm_ConfigPtr->CanNmNodeId;	/**< @req CANNM133 */

		ret_val = E_OK;
	} else {
		ret_val = E_NOT_OK;
	}

	return ret_val;
}

/**
 @brief CanNm_RepeatMessageRequest


 */
Std_ReturnType CanNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle){
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM273 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if (ModuleInternal->State != NM_STATE_REPEAT_MESSAGE
				&& ModuleInternal->Mode != NM_MODE_PREPARE_BUS_SLEEP
				&& ModuleInternal->Mode != NM_MODE_BUS_SLEEP) { /**< @req CANNM137 */

			if (ModuleInternal->State == NM_STATE_NORMAL_OPERATION || ModuleInternal->Mode == NM_STATE_READY_SLEEP) {

				ModuleInternal->Mode = NM_MODE_NETWORK;
				ModuleInternal->State = NM_STATE_REPEAT_MESSAGE; /**< @req CANNM120 *//**< @req CANNM112 */
				ModuleInternal->TxMessageSdu[CanNm_ConfigPtr->CanNmPduCbvPosition] = CANNM_CBV_REPEAT_MESSAGE_REQUEST; /**< @req CANNM121 *//**< @req CANNM113 */

				ret_val = E_OK;

			} else {
				ret_val = E_NOT_OK;
			}
		} else {
			ret_val = E_NOT_OK;
		}
	} else {
		ret_val = E_NOT_OK;
	}

	return ret_val;
}

/**
 @brief CanNm_GetPduData


 */
Std_ReturnType CanNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr){
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM275 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		memcpy(nmPduDataPtr, ModuleInternal->RxMessageSdu, ModuleInternal->PduLength);  /**< @req CANNM138 */

		ret_val = E_OK;
	} else {
		ret_val = E_NOT_OK;
	}

	return ret_val;

}

/**
 @brief CanNm_GetState


 */
Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr){
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM277 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		*nmStatePtr = ModuleInternal->State;		/**< @req CANNM091 */
		*nmStatePtr = ModuleInternal->Mode;

		ret_val = E_OK;
	} else {
		ret_val = E_NOT_OK;
	}

	return ret_val;
}

/**
 @brief CanNm_GetVersionInfo
TODO
@req CANNM224
 */
void CanNm_GetVersionInfo(Std_VersionInfoType* versioninfo){
	CanNm_InternalType* ModuleInternal = &CanNm_Internal;
	*versioninfo = ModuleInternal->VersionInfo;
}

/**
 @brief CanNm_RequestBusSynchronization
CanNmBusSynchronizationEnabled = OFF; - req CANNM280

 */
Std_ReturnType CanNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle){
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM279 */
		ret_val = E_OK;
	} else {
		ret_val = E_NOT_OK;
	}

	return ret_val;
}

/**
 @brief CanNm_CheckRemoteSleepIndication
CanNmRemoteSleepIndEnabled = OFF

 */
Std_ReturnType CanNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr){
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM281 */
		ret_val = E_OK;
	} else {
		ret_val = E_NOT_OK;
	}

	return ret_val;
}

/**
 @brief CanNm_SetSleepReadyBit
CanNmCoordinatorSyncSupport = FALSE

 */
Std_ReturnType CanNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit){
	Std_ReturnType ret_val = E_NOT_OK;

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM339 */
		ret_val = E_OK;
	} else {
		ret_val = E_NOT_OK;
	}

	return ret_val;
}


/**
 @brief CanNm_TxConfirmation


 */
void CanNm_TxConfirmation(PduIdType TxPduId, Std_ReturnType) {

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM283 */

		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		if (ModuleInternal->Mode == NM_MODE_NETWORK) {
			ModuleInternal->TimeoutTimeLeft =
					CanNm_ConfigPtr->CanNmMsgTimeoutTime; /**< @req CANNM099 */
		} else {
		}

	} else {
	}
}

/**
 @brief


 */
void CanNm_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr){

	if (InitStatus == CANNM_STATUS_INIT) { /**< @req CANNM277 */

		//CanNm_ConfigPtr
		CanNm_InternalType* ModuleInternal = &CanNm_Internal;

		memcpy(ModuleInternal->RxMessageSdu, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);  /**< @req CANNM035 */

		boolean repeatMessageBitIndication = FALSE;
		if (CanNm_ConfigPtr->CanNmPduCbvPosition != CANNM_PDU_OFF) {
			uint8 cbv = ModuleInternal->RxMessageSdu[CanNm_ConfigPtr->CanNmPduCbvPosition];
			repeatMessageBitIndication = cbv & CANNM_CBV_REPEAT_MESSAGE_REQUEST;
		}

		if (ModuleInternal->Mode == NM_MODE_BUS_SLEEP) {
			Nm_NetworkStartIndication(CanNm_ConfigPtr->NmNetworkHandle);  /**< @req CANNM127 */

		} else if (ModuleInternal->Mode == NM_MODE_PREPARE_BUS_SLEEP) {
			ModuleInternal->Mode = NM_MODE_NETWORK;		/**< @req CANNM124 */
			ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;
			ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmMsgTimeoutTime;  /**< @req CANNM096 */
			ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmRepeatMessageTime;
			ModuleInternal->MessageCycleTimeLeft = CanNm_ConfigPtr->CanNmMsgCycleTime - CanNm_ConfigPtr->CanNmMsgCycleOffset;  /**< @req CANNM100 */
			// Notify 'Network Mode'
			Nm_NetworkMode(CanNm_ConfigPtr->NmNetworkHandle);  /**< @req CANNM097 */

		} else if (ModuleInternal->Mode == NM_MODE_NETWORK) {
			ModuleInternal->TimeoutTimeLeft = CanNm_ConfigPtr->CanNmMsgTimeoutTime;  /**< @req CANNM098 */
			if (repeatMessageBitIndication) {
				if (ModuleInternal->State == NM_STATE_READY_SLEEP) {
					ModuleInternal->Mode = NM_MODE_NETWORK;
					ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;		/**< @req CANNM111 */
					ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmRepeatMessageTime;
					ModuleInternal->MessageCycleTimeLeft = CanNm_ConfigPtr->CanNmMsgCycleTime - CanNm_ConfigPtr->CanNmMsgCycleOffset;  /**< @req CANNM100 */

				} else if (ModuleInternal->State == NM_STATE_NORMAL_OPERATION) {
					ModuleInternal->Mode = NM_MODE_NETWORK;
					ModuleInternal->State = NM_STATE_REPEAT_MESSAGE;		/**< @req CANNM119 */
					ModuleInternal->RepeatMessageTimeLeft = CanNm_ConfigPtr->CanNmRepeatMessageTime;
					ModuleInternal->MessageCycleTimeLeft = CanNm_ConfigPtr->CanNmMsgCycleTime - CanNm_ConfigPtr->CanNmMsgCycleOffset;  /**< @req CANNM100 */
				} else {
					//Nothing to be done
				}
			}
		} else {
			//Nothing to be done
		}


	} else {
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
Std_ReturnType CanNm_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr){
	Std_ReturnType ret_val = E_NOT_OK;
	CanNm_InternalType* ModuleInternal = &CanNm_Internal;
	uint8 TxSdu = ModuleInternal->TxMessageSdu;

	if(sizeof(ModuleInternal->TxMessageSdu) == PduInfoPtr->SduLength){
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
	if (InitStatus == CANNM_STATUS_INIT) {

	} else{

	}
}


//TODO
/**
 * @brief Get user data offset byte index in NM Pdu
 * @param ChannelConf channel configuration
 * @return User starting data byte index in PDU
 */
static inline PduLengthType CanNm_Internal_GetUserDataOffset( const CanNm_ConfigType* InConf ) {
    PduLengthType userDataPos = 0;
    userDataPos += (InConf->CanNmPduNidPosition == CANNM_PDU_OFF) ? 0 : 1;
    userDataPos += (InConf->CanNmPduCbvPosition == CANNM_PDU_OFF) ? 0 : 1;
    return userDataPos;
}

/**
 * @brief get the ptr to user data location in Nm Pdu
 * @param ChannelConf
 * @param MessageSduPtr
 * @return pointer to user data bytes in PDU
 */
static inline uint8* CanNm_Internal_GetUserDataPtr( const CanNm_ConfigType* InConf, uint8* MessageSduPtr ) {
    PduLengthType userDataOffset = CanNm_Internal_GetUserDataOffset(InConf);
    return &MessageSduPtr[userDataOffset];
}

/**
 * @brief get user data length
 * @param ChannelConf
 * @return user data length
 */
static inline PduLengthType CanNm_Internal_GetUserDataLength( const CanNm_ConfigType* InputConf, const CanNm_InternalType* InternalConf ) {
    PduLengthType userDataOffset = CanNm_Internal_GetUserDataOffset(InputConf);
    return InternalConf->PduLength - userDataOffset;
}

//static inline void CanNm_Internal_PrepareBusSleep_to_RepeatMessage( const CanNm_ConfigType* InputConf, CanNm_InternalType* InternalConf ) {
//	InternalConf->Mode = NM_MODE_NETWORK;
//	InternalConf->State = NM_STATE_REPEAT_MESSAGE;
//	InternalConf->TimeoutTimeLeft = InputConf->CanNmTimeoutTime;  /**< @req CANNM096 */
//	InternalConf->RepeatMessageTimeLeft = InputConf->CanNmRepeatMessageTime;	/**< @req CANNM102 */
//	InternalConf->MessageCycleTimeLeft = InputConf->MessageCycleOffsetTime;  /**< @req CANNM100.2 */
//	// Notify 'Network Mode'
//	Nm_NetworkMode(InputConf->NmNetworkHandle);  /**< @req CANNM097.1 */
//}
