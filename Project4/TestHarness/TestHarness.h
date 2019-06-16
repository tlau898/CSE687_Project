#ifndef TESTHARNESS_H
#define TESTHARNESS_H

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

////////////////////////////////////////////////////////////////////////////////////////
// TestHarness.h - Implements TestHarness class that defines methods to add, clear    //
//                 and invoke a list of tests. Tests are created by parsing a test    //
//                 request and instatiating test driver objects. Test result logs     //
//                 are held by testLogger object. TestHarness supports running of     //
//                 multiple test threads by allowing a specified number of child      //
//                 threads to run test simultaneously. Test requests are sent to      //
//                 child threads as messages which are sent by a message server       //
//                 thread. Users send their test requests to the message server via   //
//                 function addTests.                                                 //
// ver 3.0                                                                            //
// Language:      Visual C++ 2010, SP1                                                //
// Application:   Project 1 CSE 687                                                   //
// Author:        Terence Lau, John Schurman                                          //
////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <ctime>
#include <list>
#include <iterator>
#include <functional>
#include <sstream>
#include <thread>

#include "TestLogger.h"
#include "TestDriverFactory.h"
#include "../Message/Message.h"
#include "../MsgPassingComm/Comm.h"

using namespace std;
using namespace MsgPassingCommunication;

class TestHarness {

public:
   TestHarness(int nTestThreads);               //Constructor, specify # child threads
   TestHarness() : TestHarness(5) {};           //Default Constructor, 5 child threads

   /* Routine to execute callable object and perform, logs status of execution */
   template <class T>
   bool executor(T&);

   /* Routine to call list of callable objects, logs status of each execution */
   template <class T>
   bool executor(list<T> &);

   ITest* createTest(string xmlTestRequest);    //Create testdriver from xml request 
   bool runTest(string testName, ITest* test);  //Run testdriver object, log result
   void start();                                //Start child test threads and main msg thread
   void addTests(string xmlTestRequest);        //Parse XML request, create tests TODEL
   void addTests(Message xmlTestRequestMsg);    //Add Tests based on message from Client
   void printLevelOneLog();                     //Print last results of runTestList, lvl1
   void printLevelTwoLog();                     //Print last results of runTestList, lvl2
   void printLevelThreeLog();                   //Print last results of runTestList, lvl3
                                                            
private:                                                                                            
   template <class T>                                       
   bool execute(string testName, T& testCase);              //Performs execution of callable object
   void MsgThreadProc();                                    //Main thread function to handle messages
   void TestThreadProc(int tID);                            //Test thread function to execute tests
   void TestManagerProc(EndPoint* serverEP, Comm* comm);    //Test handler thread function, waits on tests/requests
   string parseXMLRequest(string xmlRequest);               //Extracts body of xmlRequest 
           
   int numTestThreads;                                      //Number of child test threadss
   BlockingQueue<string> testRequests;                      //Test Request Queue
   BlockingQueue<Message> testReadyMsgs;                    //Test Ready Message Queue
   //BlockingQueue<Message> xmltestReqMsgs;
   BlockingQueue<Message> testReqMsgs;
   list<ITest*> testList;                                   //Holds List of tests to be executed
   TestLogger testLogger;                                   //Performs test logging functions
};

template<class T>
bool TestHarness::executor(T& testCase)
{
   //Clear logs from previous Executor run
   testLogger.clearlog();

   //Execute single callable object
   return execute(std::to_string(1), testCase);
}

template <class T>
bool TestHarness::executor(list<T>& testList)
{
   bool executorPassing = true;
   int testNumber = 1;

   //Clear logs from previous Executor run
   testLogger.clearlog();

   //Iterate over list of callable objects
   for (typename list<T>::iterator i = testList.begin(); i != testList.end(); i++)
   {
      //Execute single callable object
      if (!execute(std::to_string(testNumber), *i))
         executorPassing = false;
      testNumber++;
   }

   //If any callable objects fails, return false
   return executorPassing;
}

template<class T>
bool TestHarness::execute(string testName, T& testCase)
{
   try
   {
      //Try to invoke callable object
      testCase();

      //Log success, no exceptions
      testLogger.logTestStatus(testName, true);
      return true;
   }

   catch (std::runtime_error& e) {
      testLogger.logTestStatus(testName, false, e.what(), "RUNTIME_ERROR");
      return false;
   }

   catch (std::bad_cast& e) {
      testLogger.logTestStatus(testName, false, e.what(), "BAD_CAST");
      return false;
   }

   catch (std::bad_typeid& e) {
      testLogger.logTestStatus(testName, false, e.what(), "BAD_TYPEID");
      return false;
   }

   catch (std::bad_alloc& e) {
      testLogger.logTestStatus(testName, false, e.what(), "BAD_ALLOC");
      return false;
   }

   catch (std::out_of_range& e) {
      testLogger.logTestStatus(testName, false, e.what(), "OUT_OF_RANGE");
      return false;
   }

   catch (std::invalid_argument &e) {
      testLogger.logTestStatus(testName, false, e.what(), "INVALID_ARGUMENT");
      return false;
   }

   catch (std::logic_error &e) {
      testLogger.logTestStatus(testName, false, e.what(), "LOGIC_ERROR");
      return false;
   }

   catch (std::exception& e) {
      testLogger.logTestStatus(testName, false, e.what(), "GENERAL_EXCEPTION");
      return false;
   }

   catch (...) {
      testLogger.logTestStatus(testName, false, "Unknown", "Unknown Exception caught");
      return false;
   }
}

/******************************************************************************************************************
* Template: execute
* Notes:    This function is a template specialization of the execute routine which handles function pointers
*           that return a boolean value which is then intepreted as the pass/fail status of the execution.
*
******************************************************************************************************************/
template<>
inline bool TestHarness::execute<function<bool()>>(string testName, function<bool()>& testCase)
{
   try
   {
      //Try to invoke callable object
      bool status = testCase();
      testLogger.logTestStatus(testName, status);
      return status;
   }

   catch (std::runtime_error & e) {
      testLogger.logTestStatus(testName, false, e.what(), "RUNTIME_ERROR");
      return false;
   }

   catch (std::bad_cast & e) {
      testLogger.logTestStatus(testName, false, e.what(), "BAD_CAST");
      return false;
   }

   catch (std::bad_typeid & e) {
      testLogger.logTestStatus(testName, false, e.what(), "BAD_TYPEID");
      return false;
   }

   catch (std::bad_alloc & e) {
      testLogger.logTestStatus(testName, false, e.what(), "BAD_ALLOC");
      return false;
   }

   catch (std::out_of_range & e) {
      testLogger.logTestStatus(testName, false, e.what(), "OUT_OF_RANGE");
      return false;
   }

   catch (std::invalid_argument & e) {
      testLogger.logTestStatus(testName, false, e.what(), "INVALID_ARGUMENT");
      return false;
   }

   catch (std::logic_error & e) {
      testLogger.logTestStatus(testName, false, e.what(), "LOGIC_ERROR");
      return false;
   }

   catch (std::exception & e) {
      testLogger.logTestStatus(testName, false, e.what(), "GENERAL_EXCEPTION");
      return false;
   }

   catch (...) {
      testLogger.logTestStatus(testName, false, "Unknown", "Unknown Exception caught");
      return false;
   }
}

#endif //TEST_TESTHARNESS_H
