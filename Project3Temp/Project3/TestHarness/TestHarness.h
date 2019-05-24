#ifndef TESTHARNESS_H
#define TESTHARNESS_H
////////////////////////////////////////////////////////////////////////////////////////
// TestHarness.h - Implements TestHarness class that defines methods to add, clear    //
//                 and invoke a list of tests. Tests are created by parsing a test    //
//                 request and instatiating test driver objects. Test result logs     //
//                 are held by //logger object.                                         //
// ver 2.0                                                                            //
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

//#include "logger.h"
#include "TestDriverFactory.h"
#include "../Message/Message.h"
#include "../MsgPassingComm/Comm.h"

using namespace std;
using namespace MsgPassingCommunication;

class TestHarness {

public:


   TestHarness(int nTestThreads);
   TestHarness() : TestHarness(5) {};
   void add(string request);

   void start();

   /* Routine to execute callable object and perform, logs status of execution */
   template <class T>
   bool executor(T&);

   /* Routine to call list of callable objects, logs status of each execution */
   template <class T>
   bool executor(list<T> &);

   void addTests(string xmlTestRequest);      //Parse XML request, create tests
   void addTest(string testRequest);          //Create test from testDriver name
   void clearTestList();                      //Clear tests, free objects
   void runTestList();                        //Run through list of tests
   void printLevelOneLog();                   //Print last results of runTestList, lvl1
   void printLevelTwoLog();                   //Print last results of runTestList, lvl2
   void printLevelThreeLog();                 //Print last results of runTestList, lvl3

private:
   list<ITest*> testList;                     //Holds List of tests to be executed
   ////logger //logger;                             //Performs logging functions

   template <class T>
   bool execute(int testNumber, T& testCase); //Performs execution of callable object

   int numTestThreads;
   BlockingQueue<string> testQ;
   mutex testQMutex;
};

template<class T>
bool TestHarness::executor(T& testCase)
{
   //Clear logs from previous Executor run
   //logger.clearlog();

   //Execute single callable object
   return execute(1, testCase);
}

template <class T>
bool TestHarness::executor(list<T>& testList)
{
   bool executorPassing = true;
   int testNumber = 1;

   //Clear logs from previous Executor run
   //logger.clearlog();

   //Iterate over list of callable objects
   for (typename list<T>::iterator i = testList.begin(); i != testList.end(); i++)
   {
      //Execute single callable object
      if (!execute(testNumber, *i))
         executorPassing = false;
      testNumber++;
   }

   //If any callable objects fails, return false
   return executorPassing;
}

template<class T>
bool TestHarness::execute(int testNumber, T& testCase)
{
   try
   {
      //Try to invoke callable object
      testCase();

      //Log success, no exceptions
      //logger.logTestStatus(testNumber, true);
      return true;
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

#endif //TEST_TESTHARNESS_H
