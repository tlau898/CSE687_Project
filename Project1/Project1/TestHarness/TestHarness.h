#ifndef TESTHARNESS_H
#define TESTHARNESS_H
//////////////////////////////////////////////////////////////////////////////////////
// TestHarness.h - Implements TestHarness class that defines methods to invoke      //
//                 a passed callable object or a list of callable objects. Results  //
//                 are logged and available at three different levels of verbosity. //
// ver 1.0                                                                          //
// Language:      Visual C++ 2010, SP1                                              //
// Application:   Project 1 CSE 687                                                 //
// Author:        John Schurman, Terence Lau                                        //
//////////////////////////////////////////////////////////////////////////////////////

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

using namespace std;

class TestHarness {

public:
   TestHarness() = default;
   ~TestHarness() = default;
   TestHarness(const TestHarness &TH);

   /* Routine to execute callable object and perform, logs status of execution */
   template <class T>
   bool executor(T&);

   /* Routine to call list of callable objects, logs status of each execution */
   template <class T>
   bool executor(list<T> &);

   void printLevelOneLog();
   void printLevelTwoLog();
   void printLevelThreeLog();


private:
   std::vector<std::string> levelOneLog;   //Holds pass/fail status of executor call
   std::vector<std::string> levelTwoLog;   //Holds pass/fail, error message of executor call
   std::vector<std::string> levelThreeLog; //Holds pass/fail, error message, time stamp of executor call
   std::string getCurrDateTime();          //Get current date/time in M/D/Y-H:M:S

                                           //Invoked by executor, logs status for the execution of a callable object
   void logTestStatus(int testNumber, bool passed);
   void logTestStatus(int testNumber, bool passed, string exMsg, string exDetail);

   template <class T>
   bool execute(int testNumber, T& testCase); //Performs execution of callable object
};

template<class T>
bool TestHarness::executor(T& testCase)
{
   //Clear logs from previous Executor run
   levelOneLog.clear();
   levelTwoLog.clear();
   levelThreeLog.clear();

   //Execute single callable object
   return execute(1, testCase);
}

template <class T>
bool TestHarness::executor(list<T>& testList)
{
   bool executorPassing = true;
   int testNumber = 1;

   //Clear logs from previous Executor run
   levelOneLog.clear();
   levelTwoLog.clear();
   levelThreeLog.clear();

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
      logTestStatus(testNumber, true);
      return true;
   }

   catch (std::runtime_error& e) {
      logTestStatus(testNumber, false, e.what(), "RUNTIME_ERROR");
      return false;
   }

   catch (std::bad_cast& e) {
      logTestStatus(testNumber, false, e.what(), "BAD_CAST");
      return false;
   }

   catch (std::bad_typeid& e) {
      logTestStatus(testNumber, false, e.what(), "BAD_TYPEID");
      return false;
   }

   catch (std::bad_alloc& e) {
      logTestStatus(testNumber, false, e.what(), "BAD_ALLOC");
      return false;
   }

   catch (std::out_of_range& e) {
      logTestStatus(testNumber, false, e.what(), "OUT_OF_RANGE");
      return false;
   }

   catch (std::invalid_argument &e) {
      logTestStatus(testNumber, false, e.what(), "INVALID_ARGUMENT");
      return false;
   }

   catch (std::logic_error &e) {
      logTestStatus(testNumber, false, e.what(), "LOGIC_ERROR");
      return false;
   }

   catch (std::exception& e) {
      logTestStatus(testNumber, false, e.what(), "GENERAL_EXCEPTION");
      return false;
   }

   catch (...) {
      logTestStatus(testNumber, false, "Unknown", "Unknown Exception caught");
      return false;
   }
}

#endif //TEST_TESTHARNESS_H
