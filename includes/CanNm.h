#ifndef CANNM_H
#define CANNM_H

/*====================================================================================================================*/

/**===================================================================================================================*\
  @file CanNm.h

  @brief CAN Network Management
  
  Implementation of CAN Network Management.

  @see ****.pdf
\*====================================================================================================================*/

/*====================================================================================================================*\
    Załączenie nagłówków
\*====================================================================================================================*/
#include "Std_Types.h"
#include "NmStack_types.h"
#include "ComStack_Types.h"

/*====================================================================================================================*\
    Makra globalne
\*====================================================================================================================*/
#define CANNM_E_NO_INIT	0x01u /** [SWS_CanNm_00316] */
/*====================================================================================================================*\
    Typy globalne
\*====================================================================================================================*/
typedef enum {
    CANNM_PDU_BYTE_0 = 0x00,
    CANNM_PDU_BYTE_1 = 0x01,
    CANNM_PDU_OFF = 0xFF
} CanNm_PduPositionType;

typedef struct {
	PduIdType	 RxPduId;
	PduInfoType* RxPduRef;
} CanNm_RxPdu;

typedef struct {
	PduIdType	 TxConfirmationPduId;
	PduInfoType* TxPduRef;
} CanNm_TxPdu;

typedef struct {
	PduIdType	 TxUserDataPduId;
	PduInfoType* TxUserDataPduRef;
} CanNm_UserDataTxPdu;

typedef struct {
	boolean					CanNmActiveWakeupBitEnabled;
	boolean 				CanNmAllNmMessagesKeepAwake;
	boolean					CanNmBusLoadReductionActive;
	uint8					CanNmCarWakeUpBitPosition;
	uint8					CanNmCarWakeUpBytePosition;
	boolean					CanNmCarWakeUpFilterEnabled;
	uint8					CanNmCarWakeUpFilterNodeId;
	boolean					CanNmCarWakeUpRxEnabled;
	float32					CanNmImmediateNmCycleTime;
	uint8					CanNmImmediateNmTransmissions;
	float32					CanNmMsgCycleOffset;
	float32					CanNmMsgCycleTime;
	float32					CanNmMsgReducedTime;
	float32					CanNmMsgTimeoutTime;
	boolean					CanNmNodeDetectionEnabled;
	uint8					CanNmNodeId;
	boolean					CanNmNodeIdEnabled;
	CanNm_PduPositionType	CanNmPduCbvPosition;
	CanNm_PduPositionType	CanNmPduNidPosition;
	boolean					CanNmPnEnabled;
	boolean					CanNmPnEraCalcEnabled;
	boolean					CanNmPnHandleMultipleNetworkRequests;
	float32					CanNmRemoteSleepIndTime;
	float32					CanNmRepeatMessageTime;
	boolean					CanNmRepeatMsgIndEnabled;
	float32					CanNmTimeoutTime;
	float32					CanNmWaitBusSleepTime;
	NetworkHandleType 		CanNmComMNetworkHandleRef;
	PduInfoType				CanNmPnEraRxNSduRef;
	CanNm_RxPdu*			CanNmRxPdu;
	CanNm_TxPdu*			CanNmTxPdu;
	CanNm_UserDataTxPdu*	CanNmUserDataTxPdu;

} CanNm_ChannelType;

typedef struct {
	uint8 CanNmPnFilterMaskByteIndex;
	uint8 CanNmPnFilterMaskByteValue;
} CanNm_PnFilterMaskByte;

typedef struct {
	const uint8 					CanNmPnInfoLength;
	const uint8 					CanNmPnInfoOffset;
	const CanNm_PnFilterMaskByte* 	CanNmPnFilterMaskByte;
} CanNm_PnInfo;

typedef struct {
	boolean				CanNmBusLoadReductionEnabled;
	boolean				CanNmBusSynchronizationEnabled;
	CanNm_ChannelType*	CanNmChannelConfig;
	boolean				CanNmComControlEnabled;
	boolean				CanNmComUserDataSupport;
	boolean				CanNmCoordinationSyncSupport;
	boolean				CanNmDevErrorDetect;
	boolean				CanNmGlobalPnSupport;
	boolean				CanNmImmediateRestartEnabled;
	boolean				CanNmImmediateTxConfEnabled;
	float32				CanNmMainFunctionPeriod;
	boolean				CanNmPassiveModeEnabled;
	boolean				CanNmPduRxIndicationEnabled;
	boolean				CanNmPnEiraCalcEnabled;
	CanNm_PnInfo*		CanNmPnInfo;
	float32				CanNmPnResetTime;
	boolean				CanNmRemoteSleepIndEnabled;
	boolean				CanNmStateChangeIndEnabled;
	boolean				CanNmUserDataEnabled;
	boolean				CanNmVersionInfoApi;
	PduInfoType*		CanNmPnEiraRxNSduRef;
} CanNm_ConfigType;


//////////////////
typedef struct {
	Nm_ModeType Mode; /** [SWS_CanNm_00092] */
	Nm_StateType State; /** [SWS_CanNm_00094] */
	boolean Requested;
    PduLengthType PduLength;
    Std_VersionInfoType VersionInfo;
    float32 TimeoutTimeLeft;
    float32 RepeatMessageTimeLeft;
    float32 MessageCycleTimeLeft;
    float32 WaitBusSleepTimeLeft;
    uint8 TxMessageSdu[8];
    uint8 RxMessageSdu[8];
} CanNm_InternalType;

/*====================================================================================================================*\
    Eksport zmiennych globalnych
\*====================================================================================================================*/

/*====================================================================================================================*\
    Deklaracje funkcji globalnych
\*====================================================================================================================*/

/*====================================================================================================================*\
    Kod globalnych funkcji inline i makr funkcyjnych
\*====================================================================================================================*/

void CanNm_Init(const CanNm_ConfigType* cannmConfigPtr);

void CanNm_DeInit (void);

Std_ReturnType CanNm_PassiveStartUp (NetworkHandleType nmChannelHandle);

Std_ReturnType CanNm_NetworkRequest (NetworkHandleType nmChannelHandle);

Std_ReturnType CanNm_NetworkRelease (NetworkHandleType nmChannelHandle);

Std_ReturnType CanNm_DisableCommunication (NetworkHandleType nmChannelHandle);

Std_ReturnType CanNm_EnableCommunication (NetworkHandleType nmChannelHandle);

Std_ReturnType CanNm_SetUserData (NetworkHandleType nmChannelHandle, const uint8* nmUserDataPtr);

Std_ReturnType CanNm_GetUserData (NetworkHandleType nmChannelHandle, uint8* nmUserDataPtr);

Std_ReturnType CanNm_GetNodeIdentifier (NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr);

Std_ReturnType CanNm_GetLocalNodeIdentifier (NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr);

Std_ReturnType CanNm_RepeatMessageRequest (NetworkHandleType nmChannelHandle);

Std_ReturnType CanNm_GetPduData (NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr);

Std_ReturnType CanNm_GetState (NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr);

void CanNm_GetVersionInfo (Std_VersionInfoType* versioninfo);

Std_ReturnType CanNm_RequestBusSynchronization (NetworkHandleType nmChannelHandle);

Std_ReturnType CanNm_CheckRemoteSleepIndication (NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr);

Std_ReturnType CanNm_SetSleepReadyBit (NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit);

void CanNm_TxConfirmation (PduIdType TxPduId, Std_ReturnType result);

void CanNm_RxIndication (PduIdType RxPduId, const PduInfoType* PduInfoPtr);

void CanNm_ConfirmPnAvailability (NetworkHandleType nmChannelHandle);

Std_ReturnType CanNm_TriggerTransmit (PduIdType TxPduId, PduInfoType* PduInfoPtr);

void CanNm_MainFunction (void);


#endif /* CANNM_H */
