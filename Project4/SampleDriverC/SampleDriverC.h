#ifndef SAMPLEDRIVERC_H
#define SAMPLEDRIVERC_H

///////////////////////////////////////////////////////////////////////////////////////////////
// SampleDriverC.h - Defines SampleDriverC test driver class, implements RunTest routine     //
//                   and also exports Create routine to create an instance of SampleDriverC  //
// ver 1.0                                                                                   //
// Language:      Visual C++ 2010, SP1                                                       //
// Application:   Project 1 CSE 687                                                          //
// Author:        John Schurman, Terence Lau                                                 //
///////////////////////////////////////////////////////////////////////////////////////////////
#include "../TestHarness/TestDriverFactory.h"

class SampleDriverC : public ITest
{
public:
   bool RunTest();      //Return true, but generate bad cast exception
   ~SampleDriverC();

private:
};


#endif
