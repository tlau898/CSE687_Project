// SampleDriverA.cpp : Defines the exported functions for the DLL application.
//

#include "SampleDriverA.h"

using namespace std;

bool SampleDriverA::RunTest()
{
   cout << "SampleDriverA test routine invoked" << endl;
   return true;
}

DLLEXPORT void Test()
{
   cout << "Test" << endl;
}

DLLEXPORT ITest* Create()
{
   ITest* testDriverA = new SampleDriverA();
   return testDriverA;
}



