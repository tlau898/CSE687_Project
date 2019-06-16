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
   clientCnt++;
   clientPort = 8890 + clientCnt;

   serverEP = new EndPoint("localhost", 9890);
   clientEP = new EndPoint("localhost", clientPort);
   commChan = new Comm(*clientEP, "Client" + Utilities::Converter<size_t>::toString(clientCnt));

}

void Client::Start()
{
   thread msgThread(&Client::MsgThreadProc, this, clientCnt);
   msgThread.detach();
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

int Client::clientCnt = 0;

void Client::MsgThreadProc(int cID)
{
   Message msg;
   commChan->start();

   while (1)
   {
      msg = commChan->getMessage();
      MsgQueue.enQ(msg.timeStamp() + " " + msg.to().toString() + " " + commChan->name() +
         " recvd msg from " + msg.from().toString() + ": " + msg.name());

   }
}

void Client::SendRequest(string xmlTestRequest)
{
   //Send test ready message to message server
   Message msg(*serverEP, *clientEP);
   msg.name(commChan->name() + " sent test request");
   msg.timeStamp(TestLogger::getCurrDateTime());
   msg.author(commChan->name());
   msg.attribute("xmlTestRequest", xmlTestRequest);
   cout << "\n" + msg.timeStamp() + " " + clientEP->toString() + " " + msg.author() + " sent msg: " + msg.name();
   commChan->postMessage(msg);

}