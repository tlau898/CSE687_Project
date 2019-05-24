#ifndef EXECUTIVE_H
#define EXECUTIVE_H

///////////////////////////////////////////////////////////////////////////////////////////
// Executive.h - Defines Executive class which is meant to demonstrate the capabilities  //
//               of the TestHarness class.                                               //
// ver 1.0                                                                               //
// Language:      Visual C++ 2010, SP1                                                   //
// Application:   Project 1 CSE 687                                                      //
// Author:        John Schurman, Terence Lau                                             //
///////////////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <list>
#include "../TestHarness/TestHarness.h"

using namespace std;

class Executive
{
public:
   Executive(int nTestThreads) : testHarness(nTestThreads) {};
   Executive() : Executive(5) {};
   void CreateTests(string xmlTestRequest);
   void RunSampleTestSequence();

private:
   TestHarness testHarness;
};

#endif //EXECUTOR_H

