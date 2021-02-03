/** ==================================================================================================================*\
  @file UT_CanNM.c

  @brief Testy jednostkowe do CAN Network Management
\*====================================================================================================================*/
#define TEST

#include "../includes/Std_Types.h"
#include "../includes/acutest.h"
#include "../includes/fff.h"

#include "../sources/CanNM.c"
#include <string.h>



/*====================================================================================================================*\
    Makra lokalne
\*====================================================================================================================*/
#define CANNM_CHANNEL_COUNT 1
#define CANNM_RXPDU_COUNT   1
#define CANNM_SDU_LENGTH    8

/*====================================================================================================================*\
    Typy lokalne
\*====================================================================================================================*/

/*====================================================================================================================*\
    Zmienne globalne
\*====================================================================================================================*/

/*====================================================================================================================*\
    Zmienne lokalne (statyczne)
\*====================================================================================================================*/

static uint8 SduRxMessageTest[CANNM_SDU_LENGTH];
static uint8 SduTxMessageTest[CANNM_SDU_LENGTH];

CanNm_InternalType ModInternal = {
  .RxMessageSdu = 0,
  .PduLength = 0
};

PduInfoType TxPduRefTest = {
  .SduDataPtr = SduTxMessageTest,
  .SduLength = 1
};

PduInfoType RxPduRefTest = {
  .SduDataPtr = SduRxMessageTest,
  .SduLength = 1
};

static PduInfoType PduInfoPtr = {
  .SduDataPtr = 0,
  .SduLength = 1
};

static CanNm_TxPdu CanNmTxPduTest = {
  .TxConfirmationPduId = 0,
  .TxPduRef = &TxPduRefTest
};


static CanNm_RxPdu CanNmRxPduTest = {
  .RxPduId = 0,
  .RxPduRef = &TxPduRefTest
};


static CanNm_ChannelType CanNmChannelConfigTest = {
  .CanNmNodeIdEnabled = 0,
  .CanNmNodeId = 1,
  .CanNmTxPdu = &CanNmTxPduTest,
  .CanNmRxPdu = &CanNmRxPduTest,
  .CanNmPduNidPosition = CANNM_PDU_BYTE_0,
  .CanNmPduCbvPosition = CANNM_PDU_BYTE_0,
  .CanNmMsgTimeoutTime = 2000,
  .CanNmTimeoutTime = 6
};


static CanNm_ConfigType CanNm_ConfigPtrTest = {
  .CanNmChannelConfig = &CanNmChannelConfigTest,
  .CanNmMainFunctionPeriod = 5
};

NetworkHandleType nmChannelHandle = 0;
uint8 nmNodeIdPtr = 1;
Nm_StateType nmStatePtr = NM_STATE_READY_SLEEP;
Nm_ModeType nmModePtr = NM_MODE_BUS_SLEEP;
uint8 nmUserDataPtr = 1;
uint8 nmPduDataPtr = 1;
PduIdType TxPduId = 0;
PduIdType RxPduId = 0;
//Std_VersionInfoType versioninfo = moduleID;

//const uint8* nmUserDataPtr = 

//DEFINE_FFF_GLOBALS;

//FAKE_VOID_FUNC(Lib_Calc_MemS, sint32);
//FAKE_VALUE_FUNC(sint32, Lib_Calc_MemR);

/*====================================================================================================================*\
      Funkcje mock
\*====================================================================================================================*/

// FAKE_VALUE_FUNC(void *, MEMSET, void *, int, size_t);
// FAKE_VALUE_FUNC(void *, MEMCPY, void *, const void *, size_t);
// FAKE_VOID_FUNC(Nm_NetworkMode, NetworkHandleType);
// FAKE_VOID_FUNC(Nm_NetworkStartIndication, NetworkHandleType);
//FAKE_VALUE_FUNC(Std_ReturnType, Lib_Calc_Sub, sint32, sint32, sint32*);
//FAKE_VALUE_FUNC(Std_ReturnType, Lib_Calc_Mul, sint32, sint32, sint32*);
//FAKE_VALUE_FUNC(Std_ReturnType, Lib_Calc_Div, sint32, sint32, sint32*);
  

  
/** 
  @brief Test_Of_CanNm_Init

  Test function of CanNm_Init.
*/

void Test_Of_CanNm_Init(void)
{
  ///////////////////////////////  
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmNodeIdEnabled = 1;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduNidPosition = CANNM_PDU_OFF;
  CanNm_Init(&CanNm_ConfigPtrTest);

  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);
  TEST_CHECK(CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmNodeId != 		CanNm_ConfigPtr->CanNmChannelConfig->CanNmTxPdu->TxPduRef->SduDataPtr[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition]);

  ///////////////////////////////  
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmNodeIdEnabled = FALSE;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduNidPosition = CANNM_PDU_OFF;
  CanNm_Init(&CanNm_ConfigPtrTest);

  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);
  TEST_CHECK(CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmNodeId != 		CanNm_ConfigPtr->CanNmChannelConfig->CanNmTxPdu->TxPduRef->SduDataPtr[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition]);

  ///////////////////////////////  
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmNodeIdEnabled = FALSE;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduNidPosition = CANNM_PDU_BYTE_0;
  CanNm_Init(&CanNm_ConfigPtrTest);

  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);
  TEST_CHECK(CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmNodeId != 		CanNm_ConfigPtr->CanNmChannelConfig->CanNmTxPdu->TxPduRef->SduDataPtr[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition]);

  ///////////////////////////////  
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmNodeIdEnabled = 1;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduNidPosition = CANNM_PDU_BYTE_0;
  CanNm_Init(&CanNm_ConfigPtrTest);

  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);
  TEST_CHECK(CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmNodeId == 		CanNm_ConfigPtr->CanNmChannelConfig->CanNmTxPdu->TxPduRef->SduDataPtr[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition]);

}

/**
  @brief Test_Of_CanNm_DeInit

  Test function of CanNm_DeInit.
*/
void Test_Of_CanNm_DeInit (void)
{  
  //////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;
  CanNm_Internal.State = NM_STATE_BUS_SLEEP;

  CanNm_DeInit();

  TEST_CHECK(NM_STATE_UNINIT != CanNm_Internal.State);
  TEST_CHECK(CANNM_STATUS_UNINIT == InitStatus);

  //////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.State = NM_STATE_NORMAL_OPERATION;

  CanNm_DeInit();

  TEST_CHECK(NM_STATE_UNINIT != CanNm_Internal.State);
  TEST_CHECK(CANNM_STATUS_UNINIT != InitStatus);

  //////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.State = NM_STATE_BUS_SLEEP;

  CanNm_DeInit();

  TEST_CHECK(NM_STATE_UNINIT == CanNm_Internal.State);
  TEST_CHECK(CANNM_STATUS_UNINIT == InitStatus);
}

/**
  @brief Test_Of_CanNm_PassiveStartUp

  Test function of CanNm_PassiveStartUp.
*/

void Test_Of_CanNm_PassiveStartUp (void)
{
  Std_ReturnType result = E_NOT_OK;
  
  /////////////////////////////////
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;
  CanNm_Internal.State = NM_STATE_SYNCHRONIZE;
  CanNm_Init(&CanNm_ConfigPtrTest);

  result = CanNm_PassiveStartUp(nmChannelHandle);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE == CanNm_Internal.State);

  /////////////////////////////////
  CanNm_Internal.Mode = NM_MODE_PREPARE_BUS_SLEEP;
  CanNm_Internal.State = NM_STATE_SYNCHRONIZE;
  CanNm_Init(&CanNm_ConfigPtrTest);

  result = CanNm_PassiveStartUp(nmChannelHandle);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE == CanNm_Internal.State);

  ////////////////////////////////////////
  CanNm_Internal.Mode = NM_MODE_SYNCHRONIZE;
  CanNm_Internal.State = NM_STATE_SYNCHRONIZE;
  result = CanNm_PassiveStartUp(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(NM_MODE_NETWORK != CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE != CanNm_Internal.State);

  ////////////////////////////////////////
  CanNm_Internal.Mode = NM_MODE_SYNCHRONIZE;
  CanNm_Internal.State = NM_STATE_SYNCHRONIZE;
  InitStatus = CANNM_STATUS_UNINIT;
  result = CanNm_PassiveStartUp(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);  
  TEST_CHECK(NM_MODE_NETWORK != CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE != CanNm_Internal.State);
}

/**
  @brief Test_Of_CanNm_NetworkRequest

  Test function of CanNm_NetworkRequest.
*/
void Test_Of_CanNm_NetworkRequest (void)
{
  Std_ReturnType result = E_NOT_OK;

  CanNm_Init(&CanNm_ConfigPtrTest);

  /////////////////////////////////////////
  InitStatus = CANNM_STATUS_INIT; 
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;
  CanNm_Internal.State = NM_STATE_SYNCHRONIZE;
  CanNm_Internal.Requested = FALSE;

  result = CanNm_NetworkRequest(nmChannelHandle);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(TRUE == CanNm_Internal.Requested);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE == CanNm_Internal.State);

  /////////////////////////////////////////
  InitStatus = CANNM_STATUS_INIT; 
  CanNm_Internal.Mode = NM_MODE_PREPARE_BUS_SLEEP;
  CanNm_Internal.State = NM_STATE_SYNCHRONIZE;
  CanNm_Internal.Requested = FALSE;

  result = CanNm_NetworkRequest(nmChannelHandle);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(TRUE == CanNm_Internal.Requested);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE == CanNm_Internal.State);

  //////////////////////////////////////
  InitStatus = CANNM_STATUS_INIT; 
  CanNm_Internal.Mode = NM_MODE_SYNCHRONIZE;
  CanNm_Internal.State = NM_STATE_SYNCHRONIZE;
  CanNm_Internal.Requested = FALSE;

  result = CanNm_NetworkRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(TRUE == CanNm_Internal.Requested);
  TEST_CHECK(NM_MODE_NETWORK != CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE != CanNm_Internal.State);

  ///////////////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;
  CanNm_Internal.Mode = NM_MODE_SYNCHRONIZE;
  CanNm_Internal.State = NM_STATE_SYNCHRONIZE;
  CanNm_Internal.Requested = FALSE;
  
  result = CanNm_NetworkRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(FALSE == CanNm_Internal.Requested);
  TEST_CHECK(NM_MODE_NETWORK != CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE != CanNm_Internal.State);
}

/**
  @brief Test_Of_CanNm_NetworkRelease

  Test function of CanNm_NetworkRelease.
*/
void Test_Of_CanNm_NetworkRelease (void)
{
  Std_ReturnType result = E_NOT_OK;

  CanNm_Init(&CanNm_ConfigPtrTest);

  /////////////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.State = NM_STATE_NORMAL_OPERATION;
  CanNm_Internal.Requested = TRUE;

  result = CanNm_NetworkRelease(nmChannelHandle);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(FALSE == CanNm_Internal.Requested);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_READY_SLEEP == CanNm_Internal.State);

  //////////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.State = NM_STATE_BUS_SLEEP;
  CanNm_Internal.Requested = TRUE;

  result = CanNm_NetworkRelease(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(FALSE == CanNm_Internal.Requested);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_READY_SLEEP != CanNm_Internal.State);

//////////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;
  CanNm_Internal.State = NM_STATE_BUS_SLEEP;
  CanNm_Internal.Requested = TRUE;

  result = CanNm_NetworkRelease(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(FALSE == CanNm_Internal.Requested);
  TEST_CHECK(NM_MODE_NETWORK != CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_READY_SLEEP != CanNm_Internal.State);

///////////////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;
  CanNm_Internal.State = NM_STATE_BUS_SLEEP;
  CanNm_Internal.Requested = TRUE;

  result = CanNm_NetworkRelease(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(TRUE == CanNm_Internal.Requested);
  TEST_CHECK(NM_MODE_NETWORK != CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_READY_SLEEP != CanNm_Internal.State);
}

/**
  @brief Test_Of_CanNm_SetUserData

  Test function of CanNm_SetUserData.
*/

void Test_Of_CanNm_SetUserData (void)
{
  Std_ReturnType result = E_NOT_OK;

  CanNm_Init(&CanNm_ConfigPtrTest);

  /////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;

  result = CanNm_SetUserData(nmChannelHandle, &nmUserDataPtr);
  TEST_CHECK(E_OK == result);  

  /////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  result = CanNm_SetUserData(nmChannelHandle, &nmUserDataPtr);
  TEST_CHECK(E_NOT_OK == result);
}

/**
  @brief Test_Of_CanNm_GetUserData
  
  Test function of CanNm_GetUserData.
*/
void Test_Of_CanNm_GetUserData (void)
{
  Std_ReturnType result = E_NOT_OK;

  CanNm_Init(&CanNm_ConfigPtrTest);

  /////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;

  result = CanNm_GetUserData(nmChannelHandle, &nmUserDataPtr);

  TEST_CHECK(E_OK == result);  

  /////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  result = CanNm_GetUserData(nmChannelHandle, &nmUserDataPtr);

  TEST_CHECK(E_NOT_OK == result);
}

/**
  @brief Test_Of_CanNm_GetNodeIdentifier

  Test function of CanNm_GetNodeIdentifier.
*/

void Test_Of_CanNm_GetNodeIdentifier (void)
{
  Std_ReturnType result = E_NOT_OK;
  CanNm_Init(&CanNm_ConfigPtrTest);

  //////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduNidPosition = CANNM_PDU_OFF;
  
  result = CanNm_GetNodeIdentifier(nmChannelHandle, &nmNodeIdPtr);

  TEST_CHECK(E_NOT_OK == result);

  ///////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduNidPosition = CANNM_PDU_BYTE_0;
  
  result = CanNm_GetNodeIdentifier(nmChannelHandle, &nmNodeIdPtr);

  TEST_CHECK(E_OK == result);

  /////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  result = CanNm_GetNodeIdentifier(nmChannelHandle, &nmNodeIdPtr);

  TEST_CHECK(E_NOT_OK == result);
}


/**
  @brief Test_Of_CanNm_GetLocalNodeIdentifier

  Test function of CanNm_GetLocalNodeIdentifier.
*/
void Test_Of_CanNm_GetLocalNodeIdentifier (void)
{
  Std_ReturnType result = E_NOT_OK;
  CanNm_Init(&CanNm_ConfigPtrTest);

  ////////////////////////
  InitStatus = CANNM_STATUS_INIT;

  result = CanNm_GetLocalNodeIdentifier(nmChannelHandle, &nmNodeIdPtr);

  TEST_CHECK(E_OK == result);

  ////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  result = CanNm_GetLocalNodeIdentifier(nmChannelHandle, &nmNodeIdPtr);

  TEST_CHECK(E_NOT_OK == result);
}


/**
  @brief Test_Of_CanNm_RepeatMessageRequest

  Test function of CanNm_RepeatMessageRequest.
*/
void Test_Of_CanNm_RepeatMessageRequest (void)
{
  Std_ReturnType result = E_NOT_OK;
  CanNm_Init(&CanNm_ConfigPtrTest);

  ////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.State = NM_STATE_NORMAL_OPERATION;
  CanNm_Internal.Mode = NM_MODE_SYNCHRONIZE;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE == CanNm_Internal.State);

  ////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.State = NM_STATE_READY_SLEEP;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_OK == result);  
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE == CanNm_Internal.State);

  ////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.State = NM_STATE_NORMAL_OPERATION;
  CanNm_Internal.Mode = NM_MODE_PREPARE_BUS_SLEEP;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(NM_MODE_NETWORK != CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE != CanNm_Internal.State);

  ////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.State = NM_STATE_REPEAT_MESSAGE;
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(NM_MODE_NETWORK != CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE == CanNm_Internal.State);
  
  ////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.State = NM_STATE_BUS_SLEEP;
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(NM_MODE_NETWORK != CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE != CanNm_Internal.State); 

  ////////////////////////////  
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_SYNCHRONIZE;
  CanNm_Internal.State = NM_STATE_BUS_SLEEP;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(NM_MODE_NETWORK != CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE != CanNm_Internal.State); 

  ////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;
  CanNm_Internal.Mode = NM_MODE_SYNCHRONIZE;
  CanNm_Internal.State = NM_STATE_BUS_SLEEP;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);   
  TEST_CHECK(NM_MODE_NETWORK != CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE != CanNm_Internal.State);

}

/**
  @brief Test_Of_CanNm_GetPduData

  Test function of CanNm_GetPduData.
*/
void Test_Of_CanNm_GetPduData (void)
{
	Std_ReturnType result = E_NOT_OK;
  CanNm_Init(&CanNm_ConfigPtrTest);

  //////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;

  result = CanNm_GetPduData(nmChannelHandle,&nmPduDataPtr);

  TEST_CHECK(E_OK == result);

  //////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  result = CanNm_GetPduData(nmChannelHandle,&nmPduDataPtr);
  
  TEST_CHECK(E_NOT_OK == result);
}

/**
  @brief Test_Of_CanNm_GetState

  Test function of CanNm_GetState.
*/
void Test_Of_CanNm_GetState (void)
{
  Std_ReturnType result = E_NOT_OK;
  CanNm_Init(&CanNm_ConfigPtrTest);

  /////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.State = NM_STATE_PREPARE_BUS_SLEEP;
  CanNm_Internal.Mode = NM_MODE_PREPARE_BUS_SLEEP;

  result = CanNm_GetState(nmChannelHandle, &nmStatePtr, &nmModePtr);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(CanNm_Internal.State == nmStatePtr);
  TEST_CHECK(CanNm_Internal.Mode == nmModePtr);

  /////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;
  CanNm_Internal.State = NM_STATE_BUS_SLEEP;
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;

  result = CanNm_GetState(nmChannelHandle, &nmStatePtr, &nmModePtr);

  TEST_CHECK(E_NOT_OK == result); 
  TEST_CHECK(CanNm_Internal.State != nmStatePtr);
  TEST_CHECK(CanNm_Internal.Mode != nmModePtr);

}

/**
  @brief Test_Of_CanNm_GetVersionInfo

  Test function of CanNm_GetVersionInfo.
*/
void Test_Of_CanNm_GetVersionInfo (void)
{
  Std_VersionInfoType versioninfo = {
    .vendorID = 5,
    .moduleID = 1,
    .sw_major_version = 1,
    .sw_minor_version = 1,
    .sw_patch_version = 1
  };

  Std_VersionInfoType VersioninfoTest = CanNm_Internal.VersionInfo;

  CanNm_Init(&CanNm_ConfigPtrTest);
  
  CanNm_GetVersionInfo(&versioninfo);


  TEST_CHECK(CanNm_Internal.VersionInfo.vendorID == versioninfo.vendorID);
  TEST_CHECK(CanNm_Internal.VersionInfo.moduleID == versioninfo.moduleID);
  TEST_CHECK(CanNm_Internal.VersionInfo.sw_major_version == versioninfo.sw_major_version);
  TEST_CHECK(CanNm_Internal.VersionInfo.sw_minor_version == versioninfo.sw_minor_version);
  TEST_CHECK(CanNm_Internal.VersionInfo.sw_patch_version == versioninfo.sw_patch_version);
}

/**
  @brief Test_Of_CanNm_TxConfirmation

  Test function of CanNm_TxConfirmation.
*/

void Test_Of_CanNm_TxConfirmation (void)
{
  Std_ReturnType result = E_NOT_OK;
  CanNm_Init(&CanNm_ConfigPtrTest);

  ///////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  result = E_OK;
  CanNm_Internal.TimeoutTimeLeft = 0;

  CanNm_TxConfirmation(TxPduId, result);

  TEST_CHECK(CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmMsgTimeoutTime == CanNm_Internal.TimeoutTimeLeft);

  //////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;
  result = E_NOT_OK;
  CanNm_Internal.TimeoutTimeLeft = 0;

  CanNm_TxConfirmation(TxPduId, result);

  TEST_CHECK(CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmMsgTimeoutTime != CanNm_Internal.TimeoutTimeLeft);

  //////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  result = E_NOT_OK;
  CanNm_Internal.TimeoutTimeLeft = 0;

  CanNm_TxConfirmation(TxPduId, result);

  TEST_CHECK(CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmMsgTimeoutTime != CanNm_Internal.TimeoutTimeLeft);  

  //////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;
  result = E_OK;
  CanNm_Internal.TimeoutTimeLeft = 0;

  CanNm_TxConfirmation(TxPduId, result);

  TEST_CHECK(CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmMsgTimeoutTime != CanNm_Internal.TimeoutTimeLeft);  

  //////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;
  result = E_OK;
  CanNm_Internal.TimeoutTimeLeft = 0;

  CanNm_TxConfirmation(TxPduId, result);

  TEST_CHECK(CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmMsgTimeoutTime != CanNm_Internal.TimeoutTimeLeft); 
}

/**
  @brief Test_Of_CanNm_RxIndication

  Test function of CanNm_RxIndication.
*/
void Test_Of_CanNm_RxIndication (void)
{
  static uint8 PduInfoDataTest[CANNM_SDU_LENGTH];
  PduInfoType PduInfoPtrTestRx = {
  .SduDataPtr = PduInfoDataTest,
  .SduLength = 1
  };
  CanNm_Init(&CanNm_ConfigPtrTest);

  //////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduCbvPosition = CANNM_PDU_BYTE_0;
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;

  CanNm_RxIndication(RxPduId, &PduInfoPtrTestRx);

  //////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduCbvPosition = CANNM_PDU_OFF;
  CanNm_Internal.Mode = NM_MODE_PREPARE_BUS_SLEEP;

  CanNm_RxIndication(RxPduId, &PduInfoPtrTestRx);

  //////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;

  CanNm_RxIndication(RxPduId, &PduInfoPtrTestRx);  

  //////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_SYNCHRONIZE;

  CanNm_RxIndication(RxPduId, &PduInfoPtrTestRx);  

  //////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  CanNm_RxIndication(RxPduId, &PduInfoPtrTestRx);



}


/**
  @brief Test_Of_CanNm_TriggerTransmit

  Test function of CanNm_TriggerTransmit.

*/
void Test_Of_CanNm_TriggerTransmit (void)
{
  Std_ReturnType result = E_NOT_OK;
  PduInfoType PduInfoPtrTestTx;
  CanNm_InternalType* ModuleInternal = &CanNm_Internal;
  uint16 Test = sizeof(ModuleInternal->TxMessageSdu);

  CanNm_Init(&CanNm_ConfigPtrTest);

  ///////////////////////////
  PduInfoPtrTestTx.SduLength = Test;
  result = CanNm_TriggerTransmit(TxPduId, &PduInfoPtrTestTx);

  TEST_CHECK(E_OK == result);

  ///////////////////////////  
  PduInfoPtrTestTx.SduLength = Test + 1;
  result = CanNm_TriggerTransmit(TxPduId, &PduInfoPtr);

  TEST_CHECK(E_NOT_OK == result);
}

/**
  @brief Test_Of_CanNm_MainFunction

  Test function of CanNm_MainFunction.
*/
void Test_Of_CanNm_MainFunction (void)
{
  CanNm_Init(&CanNm_ConfigPtrTest);

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.TimeoutTimeLeft = CanNm_ConfigPtrTest.CanNmMainFunctionPeriod;
  CanNm_Internal.State = NM_STATE_REPEAT_MESSAGE;

  CanNm_MainFunction();

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.TimeoutTimeLeft = CanNm_ConfigPtrTest.CanNmMainFunctionPeriod;
  CanNm_Internal.State = NM_STATE_NORMAL_OPERATION;

  CanNm_MainFunction();

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.TimeoutTimeLeft = CanNm_ConfigPtrTest.CanNmMainFunctionPeriod;
  CanNm_Internal.State = NM_STATE_READY_SLEEP;

  CanNm_MainFunction();

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.TimeoutTimeLeft = CanNm_ConfigPtrTest.CanNmMainFunctionPeriod;
  CanNm_Internal.State = NM_STATE_BUS_SLEEP;

  CanNm_MainFunction();

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.State = NM_STATE_REPEAT_MESSAGE;
  CanNm_Internal.TimeoutTimeLeft = CanNm_ConfigPtrTest.CanNmMainFunctionPeriod + 1;

   CanNm_MainFunction();

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.State = NM_STATE_REPEAT_MESSAGE;
  CanNm_Internal.MessageCycleTimeLeft = CanNm_ConfigPtrTest.CanNmMainFunctionPeriod + 1;

  CanNm_Internal.Requested = TRUE;

  CanNm_MainFunction();

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.State = NM_STATE_REPEAT_MESSAGE;
  CanNm_Internal.Requested = TRUE;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduCbvPosition = CANNM_PDU_OFF;

  CanNm_MainFunction();

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.State = NM_STATE_REPEAT_MESSAGE;
  CanNm_Internal.Requested = FALSE;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduCbvPosition = CANNM_PDU_OFF;

  CanNm_MainFunction();

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.State = NM_STATE_REPEAT_MESSAGE;
  CanNm_Internal.Requested = TRUE;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduCbvPosition = CANNM_PDU_OFF;
  CanNm_Internal.RepeatMessageTimeLeft = CanNm_ConfigPtrTest.CanNmMainFunctionPeriod + 1;

  CanNm_MainFunction();

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_PREPARE_BUS_SLEEP;
  // CanNm_Internal.State = NM_STATE_NORMAL_OPERATION;
  CanNm_Internal.WaitBusSleepTimeLeft = CanNm_ConfigPtrTest.CanNmMainFunctionPeriod;

  CanNm_MainFunction();

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_PREPARE_BUS_SLEEP;
  // CanNm_Internal.State = NM_STATE_NORMAL_OPERATION;
  CanNm_Internal.WaitBusSleepTimeLeft = CanNm_ConfigPtrTest.CanNmMainFunctionPeriod + 1;

  CanNm_MainFunction();

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;

  CanNm_MainFunction();


  ///////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  CanNm_MainFunction();

}

/*
  Lista testów - wpisz tutaj wszystkie funkcje które mają być wykonane jako testy.
*/
TEST_LIST = {
    { "Test of CanNm_Init", Test_Of_CanNm_Init },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_DeInit", Test_Of_CanNm_DeInit },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_PassiveStartUp", Test_Of_CanNm_PassiveStartUp },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_NetworkRequest", Test_Of_CanNm_NetworkRequest },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_NetworkRelease", Test_Of_CanNm_NetworkRelease },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_SetUserData", Test_Of_CanNm_SetUserData },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_GetUserData", Test_Of_CanNm_GetUserData },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_Transmit", Test_Of_CanNm_Transmit },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_GetNodeIdentifier", Test_Of_CanNm_GetNodeIdentifier },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_GetLocalNodeIdentifier", Test_Of_CanNm_GetLocalNodeIdentifier },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_RepeatMessageRequest", Test_Of_CanNm_RepeatMessageRequest },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_GetPduData", Test_Of_CanNm_GetPduData },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_GetState", Test_Of_CanNm_GetState },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_GetVersionInfo", Test_Of_CanNm_GetVersionInfo },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_TxConfirmation", Test_Of_CanNm_TxConfirmation },   /* Format to { "nazwa testu", nazwa_funkcji } */    
    { "Test of CanNm_RxIndication", Test_Of_CanNm_RxIndication },   /* Format to { "nazwa testu", nazwa_funkcji } */      
    { "Test of CanNm_TriggerTransmit", Test_Of_CanNm_TriggerTransmit },   /* Format to { "nazwa testu", nazwa_funkcji } */    
    { "Test of CanNm_MainFunction", Test_Of_CanNm_MainFunction },   /* Format to { "nazwa testu", nazwa_funkcji } */    
    { NULL, NULL }                                        /* To musi być na końcu */
};
    
