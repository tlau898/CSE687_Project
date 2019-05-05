// SampleDriverB.cpp : Defines the exported functions for the DLL application.
//

#include "SampleDriverB.h"

using namespace std;

bool SampleDriverB::RunTest()
{
   cout << "SampleDriverB test routine invoked" << endl;

   //No exceptions, test passed
   return true;
}

SampleDriverB::~SampleDriverB()
{
   cout << "B Destructor was invoked" << endl;
}

DLLEXPORT ITest* Create()
{
   ITest* testDriverA = new SampleDriverB();
   return testDriverA;
}



