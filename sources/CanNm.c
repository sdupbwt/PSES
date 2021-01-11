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
/*====================================================================================================================*\
    Makra lokalne
 \*====================================================================================================================*/
#define CANNM_CBV_ACTIVE_WAKEUP_BIT 0x10
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
 @brief


 */
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle);

/**
 @brief


 */
Std_ReturnType CanNm_DisableCommunication(NetworkHandleType nmChannelHandle);

/**
 @brief


 */
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle);

/**
 @brief


 */
Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle,
		const uint8* nmUserDataPtr);

/**
 @brief


 */
Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle,
		uint8* nmUserDataPtr);

/**
 @brief


 */
Std_ReturnType CanNm_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

/**
 @brief


 */
Std_ReturnType CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle,
		uint8* nmNodeIdPtr);

/**
 @brief


 */
Std_ReturnType CanNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle,
		uint8* nmNodeIdPtr);

/**
 @brief


 */
Std_ReturnType CanNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle);

/**
 @brief


 */
Std_ReturnType CanNm_GetPduData(NetworkHandleType nmChannelHandle,
		uint8* nmPduDataPtr);

/**
 @brief


 */
Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle,
		Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr);

/**
 @brief


 */
void CanNm_GetVersionInfo(Std_VersionInfoType* versioninfo);

/**
 @brief


 */
Std_ReturnType CanNm_RequestBusSynchronization(
		NetworkHandleType nmChannelHandle);

/**
 @brief


 */
Std_ReturnType CanNm_CheckRemoteSleepIndication(
		NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr);

/**
 @brief


 */
Std_ReturnType CanNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle,
		boolean nmSleepReadyBit);

/**
 @brief


 */
Std_ReturnType CanNm_PnLearningRequest(NetworkHandleType nmChannelHandle);

/**
 @brief


 */
Std_ReturnType CanNm_RequestSynchronizedPncShutdown(
		NetworkHandleType nmChannelHandle, PNCHandleType pncId) {

}

/**
 @brief


 */
void CanNm_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);

/**
 @brief


 */
void CanNm_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

/**
 @brief


 */
void CanNm_ConfirmPnAvailability(NetworkHandleType nmChannelHandle);

/**
 @brief


 */
Std_ReturnType CanNm_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);

/**
 @brief


 */
void CanNm_MainFunction(void);


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
