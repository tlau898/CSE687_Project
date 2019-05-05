// SampleDriverA.cpp : Defines the exported functions for the DLL application.
//

#include "SampleDriverA.h"

using namespace std;

/*
* Simple function that divides by zero
* will throw an error to be caught by executor funtion
*/
void divideByZeroF() {
   int numer = 10;
   int denominator = 0;
   if (denominator == 0) {
      //NOTE: c++ does not have a built-in divideByZero Exception
      throw runtime_error("Math error- tried to divide by 0");
   }
}

bool SampleDriverA::RunTest()
{
   cout << "SampleDriverA test routine invoked" << endl;
   divideByZeroF();

   //No exceptions, test passed
   return true;
}

SampleDriverA::~SampleDriverA()
{
   cout << "SampleDriverA Destructor was invoked" << endl;
}

DLLEXPORT ITest* Create()
{
   ITest* testDriverA = new SampleDriverA();
   return testDriverA;
}



