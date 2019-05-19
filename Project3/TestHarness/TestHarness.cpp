////////////////////////////////////////////////////////////////////////////////////////
// TestHarness.cpp - Implements TestHarness class that defines methods to add, clear  //
//                   and invoke a list of tests. Tests are created by parsing a test  //
//                   request and instatiating test driver objects. Test result logs   //
//                   are held by //logger object.                                       //
// ver 2.0                                                                            //
// Language:      Visual C++ 2010, SP1                                                //
// Application:   Project 1 CSE 687                                                   //
// Author:        Terence Lau, John Schurman                                          //
////////////////////////////////////////////////////////////////////////////////////////
#include "TestHarness.h"

void TestThreadProc(int tID)
{
   Comm comm(EndPoint("localhost", 9891+tID), "client" + Utilities::Converter<size_t>::toString(tID));
   comm.start();
   EndPoint serverEP("localhost", 9890);
   EndPoint clientEP("localhost", 9891+tID);

   //while (1)
   {
      Message msg(serverEP, clientEP);
      msg.name("client #" + Utilities::Converter<size_t>::toString(tID) + " is ready to test");
      std::cout << "\n" + comm.name() + " posting:  " + msg.name();
      comm.postMessage(msg);
      Message rply = comm.getMessage();
      std::cout << "\n" + comm.name() + " received: " + rply.name();
      ::Sleep(100);
   }

   Message stop;
   stop.name("stop");
   stop.to(serverEP);
   stop.command("stop");
   comm.postMessage(stop);

   //while (1)
   //{
   //   //Send Ready Message back to TestHarness
   //   //Wait for TestRequest
   //   //Execute Test
   //   //Log Result
   //}
}

void MsgThreadProc()
{
   EndPoint serverEP("localhost", 9890);
   Comm comm(serverEP, "TestServer");
   comm.start();

   Message msg, rply;
   rply.name("reply");
   size_t count = 0;
   while (1)
   {
      msg = comm.getMessage();
      std::cout << "\n" + comm.name() + " received message: " + msg.name();

      if (msg.containsKey("file"))  // is this a file message?
      {
         if (msg.contentLength() == 0)
            std::cout << "\n  " + comm.name() + " received file \"" + msg.file() + "\" from " + msg.name();
      }
      else  // non-file message
      {
         rply.to(msg.from());
         rply.from(serverEP);
         rply.name("server test request #" + Utilities::Converter<size_t>::toString(++count) + " to " + msg.from().toString());

         comm.postMessage(rply);
         if (msg.command() == "stop")
         {
            break;
         }
      }
   }
}

TestHarness::TestHarness(int nTestThreads)
{
   numTestThreads = nTestThreads;
}

void TestHarness::start()
{
   thread msgThread(MsgThreadProc);
   msgThread.detach();

   for (int i = 1; i <= numTestThreads; i++)
   {
      thread testThread(TestThreadProc, i);
      testThread.detach();
   }
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
   string testElementDLLName;

   //Indexes used to extract info from XML
   startIndex = xmlTestRequest.find(startDelimiter);
   stopIndex = xmlTestRequest.find(stopDelimiter);

   while ((startIndex != string::npos) && (stopIndex != string::npos))
   {
      //Extract test driver dll name, create test
      startIndex += startDelimiter.length();
      testElementDLLName = xmlTestRequest.substr(startIndex, stopIndex - startIndex);
      addTest(testElementDLLName);

      //Remove already parsed dll name from test request, move to next
      xmlTestRequest.erase(0, stopIndex + stopDelimiter.length());
      startIndex = xmlTestRequest.find(startDelimiter);
      stopIndex = xmlTestRequest.find(stopDelimiter);
   }

   cout << endl;
}

/******************************************************************************************************************
* Function: addTest
* Notes:    This function takes the name a test driver dll to instantiate and add to the testList. The TestFactory
*           singleton is used to invoke the create function for the corresponding test driver object. If function
*           fails to create test driver, error message is printed.
*
******************************************************************************************************************/
void TestHarness::addTest(string testRequest)
{
   ITest* testDriverToAdd;

   //Ivoke Factory method to create instance of test driver
   testDriverToAdd = TestFactory::Instance()->CreateTestDriver(testRequest);

   if (testDriverToAdd != nullptr)
   {
      //Test Driver created successfully
      cout << "Created Test Driver " << testRequest << endl;
      testList.emplace_back(testDriverToAdd);
   }
   else
   {
      //Test Driver failed to create, print error
      cout << "Failed to load driver for " << testRequest << endl;
   }
}

/******************************************************************************************************************
* Function: clearTestList
* Notes:    This function iterates through the testList and deletes the test driver instances that were created
*           when adding tests. It then clears the test driver pointers from the testList.
*
******************************************************************************************************************/
void TestHarness::clearTestList()
{
   for (auto&& test : testList)
   {
      //Free allocated objects
      delete test;
   }
   testList.clear();
}

/******************************************************************************************************************
* Function: runTestList
* Notes:    This function iterates through the list of test driver objects in testList and creates a list of
*           lambda functions corresponding to the runTest function defined by each test driver. The list of lambda
*           functions is then invoked by executor. List of lambdas is necessary because pointers to member 
*           function cannot be passed directly to executor.
*
******************************************************************************************************************/
void TestHarness::runTestList()
{
   list<function<bool()>> tList;

   //Create list of lambdas 
   for (auto test : testList)
   {
      //Create lambda for call to test driver RunTest()
      tList.emplace_back([test]()->bool 
      {
         typedef bool (ITest::*RunTest)();
         RunTest funcPtr = &ITest::RunTest;
         return (test->*funcPtr)(); 
      });
   }

   //Clear log, execute tests
   //logger.clearlog();
   executor(tList);
}

/******************************************************************************************************************
* Function: printLevelOneLog
* Notes:    This function prints the level one log results stored by the //logger object. Log results are generated
*           when invoking runTestList and cleared on subsequent invokations.
*
******************************************************************************************************************/
void TestHarness::printLevelOneLog()
{
   //logger.printLevelOneLog();
}

/******************************************************************************************************************
* Function: printLevelTwoLog
* Notes:    This function prints the level two log results stored by the //logger object. Log results are generated
*           when invoking runTestList and cleared on subsequent invokations.
*
******************************************************************************************************************/
void TestHarness::printLevelTwoLog()
{
   //logger.printLevelTwoLog();
}

/******************************************************************************************************************
* Function: printLevelThreeLog
* Notes:    This function prints the level three log results stored by the //logger object. Log results are generated
*           when invoking runTestList and cleared on subsequent invokations.
*
******************************************************************************************************************/
void TestHarness::printLevelThreeLog()
{
   //logger.printLevelThreeLog();
}

/******************************************************************************************************************
* Template: execute 
* Notes:    This function is a template specialization of the execute routine which handles function pointers
*           that return a boolean value which is then intepreted as the pass/fail status of the execution.
*
******************************************************************************************************************/
template<>
bool TestHarness::execute<function<bool()>>(int testNumber, function<bool()>& testCase)
{
   try
   {
      //Try to invoke callable object
      bool status = testCase();
      //logger.logTestStatus(testNumber, status);
      return status;
   }

   catch (std::runtime_error& e) {
      //logger.logTestStatus(testNumber, false, e.what(), "RUNTIME_ERROR");
      return false;
   }

   catch (std::bad_cast& e) {
      //logger.logTestStatus(testNumber, false, e.what(), "BAD_CAST");
      return false;
   }

   catch (std::bad_typeid& e) {
      //logger.logTestStatus(testNumber, false, e.what(), "BAD_TYPEID");
      return false;
   }

   catch (std::bad_alloc& e) {
      //logger.logTestStatus(testNumber, false, e.what(), "BAD_ALLOC");
      return false;
   }

   catch (std::out_of_range& e) {
      //logger.logTestStatus(testNumber, false, e.what(), "OUT_OF_RANGE");
      return false;
   }

   catch (std::invalid_argument &e) {
      //logger.logTestStatus(testNumber, false, e.what(), "INVALID_ARGUMENT");
      return false;
   }

   catch (std::logic_error &e) {
      //logger.logTestStatus(testNumber, false, e.what(), "LOGIC_ERROR");
      return false;
   }

   catch (std::exception& e) {
      //logger.logTestStatus(testNumber, false, e.what(), "GENERAL_EXCEPTION");
      return false;
   }

   catch (...) {
      //logger.logTestStatus(testNumber, false, "Unknown", "Unknown Exception caught");
      return false;
   }
}

