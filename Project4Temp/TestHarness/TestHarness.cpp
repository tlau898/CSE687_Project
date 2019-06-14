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
	   Sleep(250);
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
	 
	 
      cout << "\n" + msg.timeStamp() + clientEP.toString() + " " + comm.name() + " RECVD TEST REQUEST: " +
		  rply.xmlRequest();

      //Create Test, Run Test
      ITest* test = createTest(rply.xmlRequest());
      string result = runTest(parseXMLRequest(rply.xmlRequest()), test) ? "PASS" : "FAIL";

      //Send test result message to message server
      Message testResult(serverEP, clientEP);

	  

	  //parse DLL name 
	  std::string temp = parseXMLRequest(rply.xmlRequest());
	  int pos = temp.find_last_of('\\');
	  std::string dll = temp.substr(temp.find_last_of('\\')+1  , temp.length() - pos-1);
	  //std::cout << "\n\n\n\n\nATTEMPTED DLL NAME: " << dll << std::endl;



      testResult.name(comm.name() + " Test completed Test Result: " + result);
	  testResult.dllName(dll);
      testResult.timeStamp(testLogger.getCurrDateTime());
      testResult.author(comm.name());
      testResult.to(serverEP);


	  testResult.attribute("resultready", "");

	  testResult.testResult( result);

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
	
	  //This is where the test results are printed
	  //dont go into if block if they are not test ready 
      cout << "\n" + msg.timeStamp() + " " + serverEP.toString() + " " + comm.name() + 
         " recvd msg from " + msg.from().toString() +  ": " + msg.name();
	  if (msg.containsKey("resultready")) {
		  std::thread aThread(&TestHarness::storeResult, this, msg);
		  aThread.detach();
	  }
	 
      if (msg.containsKey("testready"))
      {
         //Queue up test ready message from child test thread
         testReadyMsgs.enQ(msg);
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
	
      testReadyMsg = testReadyMsgs.deQ();    //Wait for test ready msg from child threads
	  std::cout << "Test manager" << std::endl;
	  std::string msg = testRequests.deQ();
	 
	 
      testMsg.xmlRequest(msg);  //Wait for test requests to come in from client
	  
      testMsg.timeStamp(testLogger.getCurrDateTime());  //Setup message to send to child thread
      testMsg.author(serverEP->toString());
      testMsg.to(testReadyMsg.from());     
      testMsg.from(*serverEP);

      cout << "\n" + testMsg.timeStamp() + " " + testMsg.author() + " " + comm->name() + 
         " sent msg: " + testMsg.xmlRequest() + 
         " to " + testReadyMsg.from().toString();

      comm->postMessage(testMsg);                       //Pass test request along to ready child thread
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
   //spawn off a thread to sit on blocking queue waiting for test results to come in
   thread resultThread(&TestHarness::forewardResult, this);
   resultThread.detach();

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
*           returns the created objectadd
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
	   cout << "\nCreated Test Driver " + testRequest << endl;
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
	//std::cout << "FROM TEST HARNESS: " << xmlTestRequest << std::endl;
	//std::cout << "Directory before editing " + xmlTestRequest << std::endl;

	//make it so the DLL can be loaded from the directory 
	
	
	
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

      //testRequests.enQ(testElement);

      //Remove already parsed dll name from test request, move to next
      xmlTestRequest.erase(0, stopIndex + stopDelimiter.length());
      startIndex = xmlTestRequest.find(startDelimiter);
      stopIndex = xmlTestRequest.find(stopDelimiter);
   }
   std::string dllFile = "";
   for (int i = 0; i < testElement.length(); i++) {
	   dllFile += testElement[i];
	   if (testElement[i] == 92) {
		   dllFile += (char)92;
	   }

   }
   std::cout << "\n\n\ndirectory after editing:  " + dllFile +"\n\n\n"<< std::endl;


   testRequests.enQ(dllFile);
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


void TestHarness::storeResult(Message m) {
	std::cout << "Able to store the result" << std::endl;
	results.enQ(m);
}
void TestHarness::forewardResult() {

	//need to send this result to MockChannel


	EndPoint returnResult("localhost", 4040);
	Comm comm(returnResult, "OutsideServer");
	comm.start();

	EndPoint channelEP("localhost", 4000);
	

	while (1) {
		Message m = results.deQ();
		std::cout << "\n\n\n\n\nTHE RETURN RESULT GOT DEQUEUED!!!" << std::endl;
		m.to(channelEP);
		m.from(returnResult);
		comm.postMessage(m);
		//std::thread aThread(&TestHarness::returnResult, this, m);
		//aThread.detach();
	}
}

std::string TestHarness::returnResult(Message msg) {
	
	return msg.dllName();
}

