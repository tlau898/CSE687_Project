#ifndef SAMPLEDRIVERB_H
#define SAMPLEDRIVERB_H

///////////////////////////////////////////////////////////////////////////////////////////////
// SampleDriverB.h - Defines SampleDriverB test driver class, implements RunTest routine     //
//                   and also exports Create routine to create an instance of SampleDriverB  //
// ver 1.0                                                                                   //
// Language:      Visual C++ 2010, SP1                                                       //
// Application:   Project 1 CSE 687                                                          //
// Author:        John Schurman, Terence Lau                                                 //
///////////////////////////////////////////////////////////////////////////////////////////////
#include "../TestHarness/TestDriverFactory.h"

class SampleDriverB : public ITest
{
public:
   bool RunTest();      //Return true
   ~SampleDriverB();

private:
};


#endif
