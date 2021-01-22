/** ==================================================================================================================*\
  @file UT_CanNM.c

  @brief Testy jednostkowe do CAN Network Management
\*====================================================================================================================*/
#include "../includes/Std_Types.h"
#include "../includes/acutest.h"
#include "../includes/fff.h"

#include "../sources/CanNM.c"

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


static const CanNm_ConfigType cannmConfigPtr = {
  //.CanNmActiveWakeupBitEnabled = ,
	//.CanNmAllNmMessagesKeepAwake = ,
	//.CanNmBusLoadReductionActive = ,
	//.CanNmCarWakeUpBitPosition = ,
	//.CanNmCarWakeUpBytePosition = ,
	//.CanNmCarWakeUpFilterEnabled = ,
	//.CanNmCarWakeUpFilterNodeId = ,
	//.CanNmCarWakeUpRxEnabled = ,
	//.CanNmDynamicPncToChannelMappingEnabled = ,
	//.CanNmImmediateNmCycleTime = ,
	//.CanNmImmediateNmTransmissions = ,
	//.CanNmMsgCycleOffset = ,
	//.CanNmMsgCycleTime = ,
	//.CanNmMsgReducedTime = ,
	//.CanNmMsgTimeoutTime = ,
	//.CanNmNodeDetectionEnabled = ,
	//.CanNmNodeId = ,
	//.CanNmNodeIdEnabled = ,
	//.CanNmPduCbvPosition = ,
	//.CanNmPduNidPosition = ,
	//.CanNmPnEnabled = ,
	//.CanNmPnEraCalcEnabled = ,
	//.CanNmPnHandleMultipleNetworkRequests = ,
	//.CanNmRemoteSleepIndTime = ,
	//.CanNmRepeatMessageTime = ,
	//.CanNmRepeatMsgIndEnabled = ,
	//.CanNmStayInPbsEnabled = ,
	//.CanNmSynchronizedPncShutdownEnabled = ,
	//.CanNmTimeoutTime = ,
	//.CanNmWaitBusSleepTime = ,
	//.NmNetworkHandle = ,
};

NetworkHandleType nmChannelHandle = 0;
//const uint8* nmUserDataPtr = 

//DEFINE_FFF_GLOBALS;

//FAKE_VOID_FUNC(Lib_Calc_MemS, sint32);
//FAKE_VALUE_FUNC(sint32, Lib_Calc_MemR);
//FAKE_VALUE_FUNC(Std_ReturnType, Lib_Calc_Add, sint32, sint32, sint32*);
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
  CanNm_Init(&cannmConfigPtr);
  TEST_CHECK(NM_MODE_BUS_SLEEP == CanNm_Internal.Mode);   /** @req CANNM144 */
  TEST_CHECK(NM_STATE_BUS_SLEEP == CanNm_Internal.State); /** @req CANNM141 */
  TEST_CHECK(FALSE == CanNm_Internal.Requested);          /** @req CANNM143 */ /*released*/

  //TEST_CHECK(0x00 == CanNm_Internal.TxMessageSdu);        /** @req CANNM085 */
  //TEST_CHECK(0x00 == CanNm_Internal.RxMessageSdu);        /** @req CANNM085 */

  TEST_CHECK(CANNM_STATUS_INIT == InitStatus);            /** @req CANNM060 */
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_DeInit (void)
{
  CanNm_DeInit();

  TEST_CHECK(NM_STATE_UNINIT == CanNm_Internal.State);
  TEST_CHECK(CANNM_STATUS_UNINIT == InitStatus);
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_PassiveStartUp (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_NetworkRequest (void)
{
  Std_ReturnType state = E_NOT_OK;
  CanNm_InternalType* ModuleInternal = &CanNm_Internal;

  CanNm_Init(&cannmConfigPtr);

  InitStatus = CANNM_STATUS_INIT; 
  ModuleInternal->Mode = NM_MODE_BUS_SLEEP;

/////////////////////////////////////////
  state = CanNm_NetworkRequest(nmChannelHandle);

  TEST_CHECK(E_OK == state);
  TEST_CHECK(TRUE == CanNm_Internal.Requested);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_REPEAT_MESSAGE == CanNm_Internal.State);
  TEST_CHECK(CANNM_CBV_ACTIVE_WAKEUP_BIT == CanNm_Internal.TxMessageSdu[CanNm_ConfigPtr->CanNmPduCbvPosition]);

//////////////////////////////////////
  ModuleInternal->Mode = NM_MODE_NETWORK;
  state = CanNm_NetworkRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == state);
///////////////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;
  state = CanNm_NetworkRequest(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == state);
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_NetworkRelease (void)
{
  Std_ReturnType state = E_NOT_OK;
  CanNm_InternalType* ModuleInternal = &CanNm_Internal;

  InitStatus = CANNM_STATUS_INIT;
  ModuleInternal->Mode = NM_MODE_NETWORK;
  ModuleInternal->State = NM_STATE_NORMAL_OPERATION;
  
/////////////////////////////////////////
  state = CanNm_NetworkRelease(nmChannelHandle);

  TEST_CHECK(E_OK == state);
  TEST_CHECK(FALSE == CanNm_Internal.Requested);
  TEST_CHECK(NM_MODE_NETWORK == CanNm_Internal.Mode);
  TEST_CHECK(NM_STATE_READY_SLEEP == CanNm_Internal.State);

//////////////////////////////////////
  ModuleInternal->State = NM_STATE_BUS_SLEEP;
  state = CanNm_NetworkRelease(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == state);

//////////////////////////////////////
  ModuleInternal->Mode = NM_MODE_BUS_SLEEP;
  state = CanNm_NetworkRelease(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == state);

///////////////////////////////////////////
  InitStatus = CANNM_STATUS_UNINIT;
  state = CanNm_NetworkRelease(nmChannelHandle);

  TEST_CHECK(E_NOT_OK == state);
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
/* // TODO nie trzeba chyba tego robić
void Test_Of_CanNm_DisableCommunication (void)
{

}
*/ 
/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
/* //TODO nie trzeba chyba tego robić
void Test_Of_CanNm_EnableCommunication (void)
{

}
*/

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
//TODO
/*
void Test_Of_CanNm_SetUserData (void)
{
  Std_ReturnType state = E_NOT_OK;
  CanNm_InternalType* ModuleInternal = &CanNm_Internal;

  InitStatus = CANNM_STATUS_INIT;

  InitStatus = CANNM_STATUS_UNINIT;

  state = CanNm_SetUserData(nmChannelHandle, nmUserDataPtr);
  TEST_CHECK(E_NOT_OK == CanNm_SetUserData);


}
*/
/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_GetUserData (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_Transmit (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*//*
void Test_Of_CanNm_GetNodeIdentifier (void)
{
  Std_ReturnType state =E_NOT_OK;
  CanNm_InternalType* ModuleInternal = &CanNm_Internal;

  InitStatus = CANNM_STATUS_INIT;

  state = CanNm_GetNodeIdentifier(nmChannelHandle, nmNodeIdPtr);

  TEST_CHECK(E_OK == state);
  TEST_CHECK()
}
*/
/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_GetLocalNodeIdentifier (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_RepeatMessageRequest (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_GetPduData (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_GetState (void)
{
  
}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_GetVersionInfo (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_RequestBusSynchronization (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_CheckRemoteSleepIndication (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_SetSleepReadyBit (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_PnLearningRequest (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_RequestSynchronizedPncShutdown (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_TxConfirmation (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_RxIndication (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_ConfirmPnAvailability (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_TriggerTransmit (void)
{

}

/**
  @brief Test zapisu do akumulatora

  Funkcja testująca zapis danej do akumulatora.
*/
void Test_Of_CanNm_MainFunction (void)
{

}

/*
  Lista testów - wpisz tutaj wszystkie funkcje które mają być wykonane jako testy.
*/
TEST_LIST = {
    { "Test of CanNm_Init", Test_Of_CanNm_Init },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_DeInit", Test_Of_CanNm_DeInit },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_PassiveStartUp", Test_Of_CanNm_PassiveStartUp },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_NetworkRequest", Test_Of_CanNm_NetworkRequest },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of CanNm_NetworkRelease", Test_Of_CanNm_NetworkRelease },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_DisableCommunication", Test_Of_CanNm_DisableCommunication },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_EnableCommunication", Test_Of_CanNm_EnableCommunication },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_SetUserData", Test_Of_CanNm_SetUserData },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_GetUserData", Test_Of_CanNm_GetUserData },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_Transmit", Test_Of_CanNm_Transmit },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_GetNodeIdentifier", Test_Of_CanNm_GetNodeIdentifier },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_GetLocalNodeIdentifier", Test_Of_CanNm_GetLocalNodeIdentifier },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_RepeatMessageRequest", Test_Of_CanNm_RepeatMessageRequest },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_GetPduData", Test_Of_CanNm_GetPduData },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_GetState", Test_Of_CanNm_GetState },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_GetVersionInfo", Test_Of_CanNm_GetVersionInfo },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_RequestBusSynchronization", Test_Of_CanNm_RequestBusSynchronization },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_CheckRemoteSleepIndication", Test_Of_CanNm_CheckRemoteSleepIndication },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_SetSleepReadyBit", Test_Of_CanNm_SetSleepReadyBit },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_PnLearningRequest", Test_Of_CanNm_PnLearningRequest },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_RequestSynchronizedPncShutdown", Test_Of_CanNm_RequestSynchronizedPncShutdown },   /* Format to { "nazwa testu", nazwa_funkcji } */
//    { "Test of CanNm_TxConfirmation", Test_Of_CanNm_TxConfirmation },   /* Format to { "nazwa testu", nazwa_funkcji } */    
//    { "Test of CanNm_RxIndication", Test_Of_CanNm_RxIndication },   /* Format to { "nazwa testu", nazwa_funkcji } */    
//    { "Test of CanNm_ConfirmPnAvailability", Test_Of_CanNm_ConfirmPnAvailability },   /* Format to { "nazwa testu", nazwa_funkcji } */    
//    { "Test of CanNm_TriggerTransmit", Test_Of_CanNm_TriggerTransmit },   /* Format to { "nazwa testu", nazwa_funkcji } */    
//    { "Test of CanNm_MainFunction", Test_Of_CanNm_MainFunction },   /* Format to { "nazwa testu", nazwa_funkcji } */    
    { NULL, NULL }                                        /* To musi być na końcu */
};
    
