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

CanNm_InternalType ModInternal = {
  .RxMessageSdu = 0,
  .PduLength = 0
};

PduInfoType TxPduRefTest = {
  .SduDataPtr = 0,
  .SduLength = 0
};

PduInfoType RxPduRefTest = {
  .SduDataPtr = 0,
  .SduLength = 0
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
  .CanNmNodeId = 0,
  .CanNmTxPdu = &CanNmTxPduTest,
  .CanNmRxPdu = &CanNmRxPduTest,
  .CanNmPduNidPosition = CANNM_PDU_BYTE_0,
  .CanNmPduCbvPosition = CANNM_PDU_BYTE_0,
  .CanNmMsgTimeoutTime = 2000
};


static CanNm_ConfigType CanNm_ConfigPtrTest = {
  .CanNmChannelConfig = &CanNmChannelConfigTest,
  .CanNmMainFunctionPeriod = 0
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
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
//TODO

void Test_Of_CanNm_Init(void)
{
  //TODO dodać if
  ////////////////////////////////
  // CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmNodeIdEnabled = 1;
//  CanNm_ConfigPtrTest. CanNmChannelConfig->CanNmPduNidPosition = CANNM_PDU_BYTE_0;


  CanNm_Init(&CanNm_ConfigPtrTest);

  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);
  // TEST_CHECK(CanNm_ConfigPtrTest.CanNmChannelConfig.CanNmNodeId == 		CanNm_ConfigPtr->CanNmChannelConfig->CanNmTxPdu->TxPduRef->SduDataPtr[CanNm_ConfigPtr->CanNmChannelConfig->CanNmPduNidPosition]);


}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_DeInit (void)
{
  //////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.State = NM_STATE_BUS_SLEEP;

  CanNm_DeInit();

  TEST_CHECK(NM_STATE_UNINIT == CanNm_Internal.State);
  TEST_CHECK(CANNM_STATUS_UNINIT == InitStatus);

  //////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.State = NM_STATE_NORMAL_OPERATION;

  CanNm_DeInit();

  //////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  CanNm_DeInit();
;
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/

void Test_Of_CanNm_PassiveStartUp (void)
{
  Std_ReturnType result = E_NOT_OK;  
  CanNm_InternalType* ModuleInternal = &CanNm_Internal;
  
  /////////////////////////////////
  ModuleInternal->Mode = NM_MODE_BUS_SLEEP;
  CanNm_Init(&CanNm_ConfigPtrTest);

  result = CanNm_PassiveStartUp(nmChannelHandle);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE == CanNm_Internal.State);

  /////////////////////////////////
  ModuleInternal->Mode = NM_MODE_PREPARE_BUS_SLEEP;
  CanNm_Init(&CanNm_ConfigPtrTest);

  result = CanNm_PassiveStartUp(nmChannelHandle);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE == CanNm_Internal.State);

  ////////////////////////////////////////
  ModuleInternal->Mode = NM_MODE_NETWORK;
  result = CanNm_PassiveStartUp(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result); 

  ////////////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;
  result = CanNm_PassiveStartUp(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);  
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_NetworkRequest (void)
{
  Std_ReturnType result = E_NOT_OK;
  CanNm_InternalType* ModuleInternal = &CanNm_Internal;

  CanNm_Init(&CanNm_ConfigPtrTest);

  InitStatus = CANNM_STATUS_INIT; 
  ModuleInternal->Mode = NM_MODE_BUS_SLEEP;

/////////////////////////////////////////
  result = CanNm_NetworkRequest(nmChannelHandle);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);
  TEST_CHECK(TRUE == CanNm_Internal.Requested);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE == CanNm_Internal.State);
  //TEST_CHECK(CANNM_CBV_ACTIVE_WAKEUP_BIT == CanNm_Internal.TxMessageSdu[CanNm_ConfigPtr->CanNmPduCbvPosition]);

//////////////////////////////////////
  ModuleInternal->Mode = NM_MODE_NETWORK;
  result = CanNm_NetworkRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);
///////////////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;
  result = CanNm_NetworkRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_NetworkRelease (void)
{
  Std_ReturnType result = E_NOT_OK;
  CanNm_InternalType* ModuleInternal = &CanNm_Internal;

  InitStatus = CANNM_STATUS_INIT;
  ModuleInternal->Mode = NM_MODE_NETWORK;
  ModuleInternal->State = NM_STATE_NORMAL_OPERATION;
  
/////////////////////////////////////////
  result = CanNm_NetworkRelease(nmChannelHandle);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(FALSE == CanNm_Internal.Requested);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_READY_SLEEP == CanNm_Internal.State);

//////////////////////////////////////
  ModuleInternal->State = NM_STATE_BUS_SLEEP;
  result = CanNm_NetworkRelease(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);

//////////////////////////////////////
  ModuleInternal->Mode = NM_MODE_BUS_SLEEP;
  result = CanNm_NetworkRelease(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);

///////////////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;
  result = CanNm_NetworkRelease(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
//TODO

void Test_Of_CanNm_SetUserData (void)
{
  Std_ReturnType result = E_NOT_OK;

  CanNm_Init(&CanNm_ConfigPtrTest);

  InitStatus = CANNM_STATUS_INIT;

  result = CanNm_SetUserData(nmChannelHandle, &nmUserDataPtr);
  TEST_CHECK(E_OK == result);  

  /////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  result = CanNm_SetUserData(nmChannelHandle, &nmUserDataPtr);
  TEST_CHECK(E_NOT_OK == result);
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
  TODO popraw, bo błędy
*/
void Test_Of_CanNm_GetUserData (void)
{
  Std_ReturnType result = E_NOT_OK;

  CanNm_Init(&CanNm_ConfigPtrTest);

  InitStatus = CANNM_STATUS_INIT;

  result = CanNm_GetUserData(nmChannelHandle, &nmUserDataPtr);
  TEST_CHECK(E_OK == result);  

  /////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  result = CanNm_GetUserData(nmChannelHandle, &nmUserDataPtr);
  TEST_CHECK(E_NOT_OK == result);
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/

void Test_Of_CanNm_GetNodeIdentifier (void)
{
  Std_ReturnType result =E_NOT_OK;


//////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduNidPosition = CANNM_PDU_OFF;

  CanNm_Init(&CanNm_ConfigPtrTest);
  
  result = CanNm_GetNodeIdentifier(nmChannelHandle, &nmNodeIdPtr);

  TEST_CHECK(E_NOT_OK == result);
  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);

  ///////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduNidPosition = CANNM_PDU_BYTE_0;

  CanNm_Init(&CanNm_ConfigPtrTest);
  
  result = CanNm_GetNodeIdentifier(nmChannelHandle, &nmNodeIdPtr);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);
  // TODO sprawdź wartość
  //TEST_CHECK( == *nmNodeIdPtr);

  /////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  result = CanNm_GetNodeIdentifier(nmChannelHandle, &nmNodeIdPtr);

  TEST_CHECK(E_NOT_OK == result);
}


/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_GetLocalNodeIdentifier (void)
{
  Std_ReturnType result = E_NOT_OK;

  CanNm_Init(&CanNm_ConfigPtrTest);
  ////////////////////////
  InitStatus = CANNM_STATUS_INIT;

  result = CanNm_GetLocalNodeIdentifier(nmChannelHandle, &nmNodeIdPtr);

  TEST_CHECK(E_OK == result);
  //TODO sprawdź wartość
  //TEST_CHECK( == *nmNodeIdPtr);

  ////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  result = CanNm_GetLocalNodeIdentifier(nmChannelHandle, &nmNodeIdPtr);

  TEST_CHECK(E_NOT_OK == result);
}


/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_RepeatMessageRequest (void)
{
  Std_ReturnType result = E_NOT_OK;
  CanNm_InternalType* ModuleInternal = &CanNm_Internal;

  CanNm_Init(&CanNm_ConfigPtrTest);

  ////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  ModuleInternal->State = NM_STATE_NORMAL_OPERATION;
  ModuleInternal->Mode = NM_MODE_NETWORK;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE == CanNm_Internal.State);

  ////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  ModuleInternal->State = NM_STATE_NORMAL_OPERATION;
  ModuleInternal->Mode = NM_MODE_PREPARE_BUS_SLEEP;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);

  ////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  ModuleInternal->State = NM_STATE_NORMAL_OPERATION;
  ModuleInternal->Mode = NM_MODE_BUS_SLEEP;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);
  
  ////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  ModuleInternal->State = NM_STATE_BUS_SLEEP;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);

  ////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  ModuleInternal->Mode = NM_MODE_NETWORK;
  ModuleInternal->State = NM_STATE_NORMAL_OPERATION;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_OK == result);  

  ////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  ModuleInternal->Mode = NM_MODE_NETWORK;
  ModuleInternal->State = NM_STATE_READY_SLEEP;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_OK == result);  

  ////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  ModuleInternal->Mode = NM_MODE_NETWORK;
  ModuleInternal->State = NM_STATE_BUS_SLEEP;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);

  ////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;
  
  result = CanNm_RepeatMessageRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == result);   

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
//TODO
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
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_GetState (void)
{
  Std_ReturnType result = E_NOT_OK;

  CanNm_Internal.State = NM_STATE_PREPARE_BUS_SLEEP;
  CanNm_Internal.Mode = NM_MODE_PREPARE_BUS_SLEEP;

  CanNm_Init(&CanNm_ConfigPtrTest);

  /////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;

  result = CanNm_GetState(nmChannelHandle, &nmStatePtr, &nmModePtr);

  TEST_CHECK(E_OK == result);
  TEST_CHECK(CanNm_Internal.State == nmStatePtr);
  TEST_CHECK(CanNm_Internal.Mode == nmModePtr);

  /////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  result = CanNm_GetState(nmChannelHandle, &nmStatePtr, &nmModePtr);

  TEST_CHECK(E_NOT_OK == result); 

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
//TODO
void Test_Of_CanNm_GetVersionInfo (void)
{
  Std_VersionInfoType versioninfo;

  CanNm_Init(&CanNm_ConfigPtrTest);
  CanNm_GetVersionInfo(&versioninfo);

// TODO wskaźniki
  // TEST_CHECK(CanNm_Internal.VersionInfo == versioninfo);
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
// TODO, zapytaj, czy na pewno tak ma być

void Test_Of_CanNm_TxConfirmation (void)
{
  Std_ReturnType result = E_NOT_OK;
  CanNm_Init(&CanNm_ConfigPtrTest);

  ///////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;

  CanNm_TxConfirmation(TxPduId, result);

//TODO coś nie działa
  // TEST_CHECK(CanNm_ConfigPtrTest.CanNmChannelConfig.CanNmMsgTimeoutTime == CanNm_Internal.TimeoutTimeLeft);

  //////////////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;

  CanNm_TxConfirmation(TxPduId, result);

  //////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  CanNm_TxConfirmation(TxPduId, result);

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_RxIndication (void)
{
  PduInfoType PduInfoPtrTest;
  CanNm_Init(&CanNm_ConfigPtrTest);

  //////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduCbvPosition = CANNM_PDU_BYTE_0;
  CanNm_Internal.Mode = NM_MODE_BUS_SLEEP;

  CanNm_RxIndication(RxPduId, &PduInfoPtrTest);

  //////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_ConfigPtrTest.CanNmChannelConfig->CanNmPduCbvPosition = CANNM_PDU_OFF;
  CanNm_Internal.Mode = NM_MODE_PREPARE_BUS_SLEEP;

  CanNm_RxIndication(RxPduId, &PduInfoPtrTest);

  //////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;

  CanNm_RxIndication(RxPduId, &PduInfoPtrTest);  

  //////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_SYNCHRONIZE;

  CanNm_RxIndication(RxPduId, &PduInfoPtrTest);  

  //////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;

  CanNm_RxIndication(RxPduId, &PduInfoPtrTest);



}


/**
  @brief Test zapisu do akumulatora

TODO popraw
*/
void Test_Of_CanNm_TriggerTransmit (void)
{
  Std_ReturnType result = E_NOT_OK;
  PduInfoType PduInfoPtrTest2 = {
    .SduLength = 1,
    .SduDataPtr = 0
  };

  CanNm_Init(&CanNm_ConfigPtrTest);

  ///////////////////////////
  // PduInfoPtrTest2.SduLength = sizeof(CanNm_Internal.TxMessageSdu);
  result = CanNm_TriggerTransmit(TxPduId, &PduInfoPtrTest2);

  // TEST_CHECK(E_OK == result);

  ///////////////////////////  
  // PduInfoPtr.SduLength = sizeof(CanNm_Internal.TxMessageSdu) + 1;
  result = CanNm_TriggerTransmit(TxPduId, &PduInfoPtr);

  TEST_CHECK(E_NOT_OK == result);
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
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

  // ///////////////////////////
  // InitStatus = CANNM_STATUS_INIT;
  // CanNm_Internal.Mode = NM_MODE_NETWORK;
  // // CanNm_Internal.TimeoutTimeLeft = CanNm_ConfigPtrTest.CanNmMainFunctionPeriod - 1;
  // CanNm_ConfigPtrTest.CanNmMainFunctionPeriod = CanNm_Internal.TimeoutTimeLeft + 1;

  // CanNm_Init(&CanNm_ConfigPtrTest);

  //  CanNm_MainFunction();

  ///////////////////////////
  InitStatus = CANNM_STATUS_INIT;
  CanNm_Internal.Mode = NM_MODE_NETWORK;
  CanNm_Internal.State = NM_STATE_REPEAT_MESSAGE;

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
  CanNm_Internal.State = NM_STATE_NORMAL_OPERATION;

  CanNm_MainFunction();





  // ///////////////////////////
  // InitStatus = CANNM_STATUS_UNINIT;

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
    
