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

/*====================================================================================================================*\
    Typy globalne
\*====================================================================================================================*/
typedef struct {
  
} CanNm_ConfigType;

/*====================================================================================================================*\
    Eksport zmiennych globalnych
\*====================================================================================================================*/

/*====================================================================================================================*\
    Deklaracje funkcji globalnych
\*====================================================================================================================*/
void Calc_Reset(void);

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

Std_ReturnType CanNm_Transmit (PduIdType TxPduId, const PduInfoType* PduInfoPtr);

Std_ReturnType CanNm_GetNodeIdentifier (NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr);

Std_ReturnType CanNm_GetLocalNodeIdentifier (NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr);

Std_ReturnType CanNm_RepeatMessageRequest (NetworkHandleType nmChannelHandle);

Std_ReturnType CanNm_GetPduData (NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr);

Std_ReturnType CanNm_GetState (NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr);

void CanNm_GetVersionInfo (Std_VersionInfoType* versioninfo);

Std_ReturnType CanNm_RequestBusSynchronization (NetworkHandleType nmChannelHandle);

Std_ReturnType CanNm_CheckRemoteSleepIndication (NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr);

Std_ReturnType CanNm_SetSleepReadyBit (NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit);

Std_ReturnType CanNm_PnLearningRequest (NetworkHandleType nmChannelHandle);

Std_ReturnType CanNm_RequestSynchronizedPncShutdown (NetworkHandleType nmChannelHandle, PNCHandleType pncId);

void CanNm_TxConfirmation (PduIdType TxPduId, Std_ReturnType result);

void CanNm_RxIndication (PduIdType RxPduId, const PduInfoType* PduInfoPtr);

void CanNm_ConfirmPnAvailability (NetworkHandleType nmChannelHandle);

Std_ReturnType CanNm_TriggerTransmit (PduIdType TxPduId, PduInfoType* PduInfoPtr);

void CanNm_MainFunction (void);



#endif /* CANNM_H */
