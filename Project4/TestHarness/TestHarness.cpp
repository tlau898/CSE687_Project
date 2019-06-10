//////////////////////////////////////////////////////////////////////////////////////////
// TestHarness.cpp - Implements TestHarness class that defines methods to add, clear    //
//                   and invoke a list of tests. Tests are created by parsing a test    //
//                   request and instatiating test driver objects. Test result logs     //
//                   are held by testLogger object. TestHarness supports running of     //
//                   multiple test threads by allowing a specified number of child      //
//                   threads to run test simultaneously. Test requests are sent to      //
//                   child threads as messages which are sent by a message server       //
//                   thread. Users send their test requests to the message server via   //
//                   function addTests.                                                 //
// ver 3.0                                                                              //
// Language:      Visual C++ 2010, SP1                                                  //
// Application:   Project 1 CSE 687                                                     //
// Author:        Terence Lau, John Schurman                                            //
//////////////////////////////////////////////////////////////////////////////////////////
#include "TestHarness.h"

/******************************************************************************************************************
* Constructor: 
* Notes:    The constructor takes in a specified number of test threads which defines how many test threads will
*           be spawned when the start function is initiated.
*
******************************************************************************************************************/
TestHarness::TestHarness(int nTestThreads)
{
   //Number of child test threads to spawn
   numTestThreads = nTestThreads;
}

/******************************************************************************************************************
* Function: TestThreadProc
* Notes:    This function is run by each of the child test threads that are spawned by the start function. This
*           function communicates with the test server thread by first posting a ready message to the test harness
*           indicating the child test thread is ready to test. The child test thread then waits for a response
*           message from the TestHarness thread containing a xml test request. Upon receiving the xml test request
*           the child test thread then creates and runs the test described by the test request. It then sends a 
*           results message back to the TestHarness thread and then repeats the whole cycle again.
*
******************************************************************************************************************/
void TestHarness::TestThreadProc(int tID)
{
   Comm comm(EndPoint("localhost", 9890+tID), "Child" + Utilities::Converter<size_t>::toString(tID));
   comm.start();
   EndPoint serverEP("localhost", 9890);        //Main TestHarness server end point, msg processing thread
   EndPoint clientEP("localhost", 9890+tID);    //Child Test Thread end point

   while (1)
   {
      //Send test ready message to message server
      Message msg(serverEP, clientEP);
      msg.name(comm.name() + " is ready to test");
      msg.timeStamp(testLogger.getCurrDateTime());
      msg.author(comm.name());
      msg.attribute("testready", "");
      cout << "\n" + msg.timeStamp() + " " + clientEP.toString() + " " + msg.author() + " sent msg: " + msg.name();
      comm.postMessage(msg);

      //Wait for test request message
      Message rply = comm.getMessage();
      cout << "\n" + msg.timeStamp() + clientEP.toString() + " " + comm.name() + " recvd test request: " + rply.xmlRequest();

      //Create Test, Run Test
      ITest* test = createTest(rply.xmlTestRequest());
      string result = runTest(parseXMLRequest(rply.xmlTestRequest()), test) ? "PASS" : "FAIL";

      //Send test result message to message server
      Message testResult(serverEP, clientEP);
      testResult.attribute("testResult", result);
      testResult.name(comm.name() + " Test completed Test Result: " + result);
      testResult.timeStamp(testLogger.getCurrDateTime());
      testResult.author(comm.name());
      testResult.to(serverEP);
      comm.postMessage(testResult);
   }
}

/******************************************************************************************************************
* Function: MsgThreadProc
* Notes:    This function is the main TestHarness thread which is responsible for processing messages from child
*           test threads which post test ready messages and test result messages. This thread spawns the test
*           manager thread which waits for a testReady message and testRequest message. This allows the message 
*           thread to not have to block and makes it free to process messages as they come in.
*
******************************************************************************************************************/
void TestHarness::MsgThreadProc()
{
   Message msg, rply;
   EndPoint serverEP("localhost", 9890);
   Comm comm(serverEP, "TestServer");
   comm.start();

   //Spawn test handler thread, waits for a a testReady message and testRequest message
   thread testManagerThread(&TestHarness::TestManagerProc, this, &serverEP, &comm);
   testManagerThread.detach();

   while (1)
   {
      //Print contents of incoming messages
      msg = comm.getMessage();
      cout << "\n" + msg.timeStamp() + " " + serverEP.toString() + " " + comm.name() + 
         " recvd msg from " + msg.from().toString() +  ": " + msg.name();
      if (msg.containsKey("testready"))
      {
         //Queue up test ready message from child test thread
         testReadyMsgs.enQ(msg);
      }
      else if (msg.containsKey("xmlTestRequest"))
      {
         //testReqMsgs.enQ(msg);
         //turn xml request into multiple single test req msgs
         this->addTests(msg);
      }
      else if (msg.containsKey("testResult"))
      {
         //Send test ready message to message server
         EndPoint clientEP("localhost", 8891);
         Message resultMsg(clientEP, serverEP);
         resultMsg.name(comm.name() + " " + msg.testResult());
         resultMsg.timeStamp(TestLogger::getCurrDateTime());
         resultMsg.author(comm.name());
         resultMsg.to(clientEP);
         comm.postMessage(resultMsg);
      }
   }
}

/******************************************************************************************************************
* Function: TestManagerProc
* Notes:    This function is run as the test manager thread and spawned by the main TestHarness message processing
*           thread. It's purpose is to free up the message processing thread from having to block on ready messages
*           from the child test threads and test request messages from clients. This function waits for a ready 
*           message from a child thread and a test request to come in. It will then send the test request to the
*           child thread that sent the test ready message.
*
******************************************************************************************************************/
void TestHarness::TestManagerProc(EndPoint* serverEP, Comm* comm)
{
   Message testReadyMsg; //Test Request message sent to testServer
   Message testMsg;      //Test Request message sent to child process

   while (1)
   {
      testReadyMsg = testReadyMsgs.deQ();               //Wait for test ready msg from child threads
      testMsg = testReqMsgs.deQ();
      //testMsg.xmlRequest(testRequests.deQ());           //Wait for test requests to come in from client
      testMsg.timeStamp(testLogger.getCurrDateTime());  //Setup message to send to child thread
      //testMsg.author(serverEP->toString());
      testMsg.to(testReadyMsg.from());
      //testMsg.from(*serverEP);

      cout << "\n" + testMsg.timeStamp() + " " + testMsg.author() + " " + comm->name() +
         " sent msg: " + testMsg.xmlTestRequest() +
         " to " + testReadyMsg.from().toString();

      //comm->postMessage(testMsg);                       //Pass test request along to ready child thread

      //testReadyMsg = testReadyMsgs.deQ();               //Wait for test ready msg from child threads
      //testMsg.xmlRequest(testRequests.deQ());           //Wait for test requests to come in from client
      //testMsg.timeStamp(testLogger.getCurrDateTime());  //Setup message to send to child thread
      //testMsg.author(serverEP->toString());
      //testMsg.to(testReadyMsg.from());     
      //testMsg.from(*serverEP);

      //cout << "\n" + testMsg.timeStamp() + " " + testMsg.author() + " " + comm->name() + 
      //   " sent msg: " + testMsg.xmlRequest() + 
      //   " to " + testReadyMsg.from().toString();

      comm->postMessage(testMsg);                       //Pass test request along to ready child thread
   }
}

void TestHarness::addTests(Message xmlTestRequestMsg)
{
   string startDelimiter = "<testelement>"; //XML
   string stopDelimiter = "</testelement>"; //XML
   size_t startIndex;
   size_t stopIndex;
   string testElement;
   string xmlReq;
   Message testRequestMsg;

   testRequestMsg = xmlTestRequestMsg;
   xmlReq = xmlTestRequestMsg.xmlTestRequest();

   //Indexes used to extract info from XML
   startIndex = xmlReq.find(startDelimiter);
   stopIndex = xmlReq.find(stopDelimiter);

   while ((startIndex != string::npos) && (stopIndex != string::npos))
   {
      //Extract test driver dll name, create test
      testElement = xmlReq.substr(startIndex, stopIndex + stopDelimiter.length());
      testRequestMsg.attribute("xmlTestRequest", testElement);
      testReqMsgs.enQ(testRequestMsg);
      //testRequests.enQ(testElement);

      //Remove already parsed dll name from test request, move to next
      xmlReq.erase(0, stopIndex + stopDelimiter.length());
      startIndex = xmlReq.find(startDelimiter);
      stopIndex = xmlReq.find(stopDelimiter);
   }
}

/******************************************************************************************************************
* Function: start
* Notes:    This function creates the TestHarness's main message processing thread that handles requests from 
*           clients and messages from the child test threads. This function also spawns child test threads up to
*           the number specified by numTestThreads passed into the constructor.
*
******************************************************************************************************************/
void TestHarness::start()
{
   //Spawn main message processing thread
   thread msgThread(&TestHarness::MsgThreadProc, this);
   msgThread.detach();

   for (int i = 1; i <= numTestThreads; i++)
   {
      //Spawn child test thread
      thread testThread(&TestHarness::TestThreadProc, this, i);
      testThread.detach();
   }
}

/******************************************************************************************************************
* Function: createTest
* Notes:    This function parses an XML test request, creates the corresponding test driver object and then
*           returns the created object.
*
******************************************************************************************************************/
ITest* TestHarness::createTest(string xmlTestRequest)
{
   ITest* testDriverToAdd;
   string testRequest;

   //Ivoke Factory method to create instance of test driver
   testRequest = parseXMLRequest(xmlTestRequest);
   testDriverToAdd = TestFactory::Instance()->CreateTestDriver(testRequest);

   if (testDriverToAdd != nullptr)
   {
      //Test Driver created successfully
      cout << "\nCreated Test Driver " + testRequest;
   }
   else
   {
      //Test Driver failed to create, print error
      cout << "Failed to load driver for " << testRequest << endl;
   }
   return testDriverToAdd;
}

/******************************************************************************************************************
* Function: runTest
* Notes:    This function executes the TestDriver's RunTest function.
*
******************************************************************************************************************/
bool TestHarness::runTest(string testName, ITest* test)
{
   function<bool()> lambda = [test]()->bool
   {
      typedef bool (ITest:: * RunTest)();
      RunTest funcPtr = &ITest::RunTest;
      return (test->*funcPtr)();
   };
   return execute(testName, lambda);
}

/******************************************************************************************************************
* Function: parseXMLRequest
* Notes:    This function strips away the xml portion of the xmlRequest and returns the body of the request
*
******************************************************************************************************************/
string TestHarness::parseXMLRequest(string xmlRequest)
{
   string startDelimiter = "<testelement>"; //XML
   string stopDelimiter = "</testelement>"; //XML
   size_t startIndex;
   size_t stopIndex;
   string testElementDLLName;

   //Indexes used to extract info from XML
   startIndex = xmlRequest.find(startDelimiter);
   stopIndex = xmlRequest.find(stopDelimiter);

   //Extract test driver dll name, create test
   startIndex += startDelimiter.length();
   testElementDLLName = xmlRequest.substr(startIndex, stopIndex - startIndex);
   return testElementDLLName;
}

/******************************************************************************************************************
* Function: addTests
* Notes:    This function takes an xml string corresponding to a test request and parses for test driver dll 
*           names. Each test driver dll name found is passed to addTest which will attempt to load the dll and
*           create an instance of the test driver to add to the testList.
*
******************************************************************************************************************/
void TestHarness::addTests(string xmlTestRequest)
{
   string startDelimiter = "<testelement>"; //XML
   string stopDelimiter = "</testelement>"; //XML
   size_t startIndex;
   size_t stopIndex;
   string testElement;

   //Indexes used to extract info from XML
   startIndex = xmlTestRequest.find(startDelimiter);
   stopIndex = xmlTestRequest.find(stopDelimiter);

   while ((startIndex != string::npos) && (stopIndex != string::npos))
   {
      //Extract test driver dll name, create test
      testElement = xmlTestRequest.substr(startIndex, stopIndex + stopDelimiter.length());
      testRequests.enQ(testElement);

      //Remove already parsed dll name from test request, move to next
      xmlTestRequest.erase(0, stopIndex + stopDelimiter.length());
      startIndex = xmlTestRequest.find(startDelimiter);
      stopIndex = xmlTestRequest.find(stopDelimiter);
   }
}

/******************************************************************************************************************
* Function: printLevelOneLog
* Notes:    This function prints the level one log results stored by the testLogger object. Log results are generated
*           when invoking runTestList and cleared on subsequent invokations.
*
******************************************************************************************************************/
void TestHarness::printLevelOneLog()
{
   testLogger.printLevelOneLog();
}

/******************************************************************************************************************
* Function: printLevelTwoLog
* Notes:    This function prints the level two log results stored by the testLogger object. Log results are generated
*           when invoking runTestList and cleared on subsequent invokations.
*
******************************************************************************************************************/
void TestHarness::printLevelTwoLog()
{
   testLogger.printLevelTwoLog();
}

/******************************************************************************************************************
* Function: printLevelThreeLog
* Notes:    This function prints the level three log results stored by the testLogger object. Log results are generated
*           when invoking runTestList and cleared on subsequent invokations.
*
******************************************************************************************************************/
void TestHarness::printLevelThreeLog()
{
   testLogger.printLevelThreeLog();
}

