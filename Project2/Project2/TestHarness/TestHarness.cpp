////////////////////////////////////////////////////////////////////////////////////////
// TestHarness.cpp - Implements TestHarness class that defines methods to invoke      //
//                   a passed callable object or a list of callable objects. Results  //
//                   are logged and available at three different levels of verbosity. //
// ver 1.0                                                                            //
// Language:      Visual C++ 2010, SP1                                                //
// Application:   Project 1 CSE 687                                                   //
// Author:        John Schurman, Terence Lau                                          //
////////////////////////////////////////////////////////////////////////////////////////

#include "TestHarness.h"

void TestHarness::AddTest(string testRequest)
{
   ITest* testDriverToAdd;
   testDriverToAdd = TestFactory::Instance()->CreateTestDriver(testRequest);

   if (testDriverToAdd != nullptr)
   {
      testList.emplace_back(testDriverToAdd);
   }
   else
   {
      cout << "Failed to load driver for " << testRequest << endl;
   }
}

void TestHarness::AddTests(string xmlTestRequest)
{
   string startDelimiter = "<testelement>";
   string stopDelimiter = "</testelement>";
   size_t startIndex;
   size_t stopIndex;
   string testElementDLLName;

   startIndex = xmlTestRequest.find(startDelimiter);
   stopIndex = xmlTestRequest.find(stopDelimiter);

   while ((startIndex != string::npos) && (stopIndex != string::npos))
   {
      startIndex += startDelimiter.length();
      testElementDLLName = xmlTestRequest.substr(startIndex, stopIndex - startIndex);
      AddTest(testElementDLLName);

      xmlTestRequest.erase(0, stopIndex + stopDelimiter.length());
      startIndex = xmlTestRequest.find(startDelimiter);
      stopIndex = xmlTestRequest.find(stopDelimiter);
   }
}

void TestHarness::ClearTestList()
{
   for (auto&& test : testList)
   {
      delete test;
   }
   testList.clear();
}

void TestHarness::RunTestList()
{
   logger.clearlog();
   for (auto test : testList)
   {
      test->RunTest();
   }
}

void TestHarness::printLevelOneLog()
{
   logger.printLevelOneLog();
}

void TestHarness::printLevelTwoLog()
{
   logger.printLevelTwoLog();
}

void TestHarness::printLevelThreeLog()
{
   logger.printLevelThreeLog();
}

