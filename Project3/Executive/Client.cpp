/////////////////////////////////////////////////////////////////////////////////////////////
// Client.cpp - Defines Client class which is meant to post test request messages to the   //
//              test harness.                                                              //
// ver 1.0                                                                                 //
// Language:      Visual C++ 2010, SP1                                                     //
// Application:   Project 1 CSE 687                                                        //
// Author:        John Schurman, Terence Lau                                               //
/////////////////////////////////////////////////////////////////////////////////////////////
#include "Client.h"

/******************************************************************************************************************
* Constructor
* Notes:    This constructor takes in a pointer to a TestHarness object to be used to post test requests to.
*
******************************************************************************************************************/
Client::Client(TestHarness* testHarness)
{
   tHarness = testHarness;
}

/******************************************************************************************************************
* Function: AddTests
* Notes:    This function invokes the testHarness addTests function which parses an xmlTestRequest string, loads
*           the corresponding test driver dlls and then creates a list of test driver objects.
*
******************************************************************************************************************/
void Client::AddTests(string xmlTestRequest)
{
   tHarness->addTests(xmlTestRequest);
}