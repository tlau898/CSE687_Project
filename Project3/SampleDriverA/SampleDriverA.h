#ifndef SAMPLEDRIVERA_H
#define SAMPLEDRIVERA_H

///////////////////////////////////////////////////////////////////////////////////////////////
// SampleDriverA.h - Defines SampleDriverA test driver class, implements RunTest routine     //
//                   and also exports Create routine to create an instance of SampleDriverA  //
// ver 1.0                                                                                   //
// Language:      Visual C++ 2010, SP1                                                       //
// Application:   Project 1 CSE 687                                                          //
// Author:        John Schurman, Terence Lau                                                 //
///////////////////////////////////////////////////////////////////////////////////////////////
#include "../TestHarness/TestDriverFactory.h"

class SampleDriverA : public ITest
{
public:
   bool RunTest();      //Return false, no exceptions generated
   ~SampleDriverA();

private:
};


#endif
