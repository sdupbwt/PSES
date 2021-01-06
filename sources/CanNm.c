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
} CanNm_Internal_ChannelType;

/*====================================================================================================================*\
    Zmienne globalne
 \*====================================================================================================================*/
CanNm_InitStatusType InitStatus;
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
void CanNm_Init(const CanNm_ConfigType* cannmConfigPtr) {

}

/**
 @brief


 */
void CanNm_DeInit(void);

/**
 @brief


 */
Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle);

/**
 @brief


 */
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle);

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
