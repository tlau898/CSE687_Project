// SandBox.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <Windows.h>
#include "../TestDriverFactory/TestDriverFactory.h"

using namespace std;

int main()
{
   TestFactory* testFactory = TestFactory::Instance();
   ITest* testDriverHandle;

   testDriverHandle = testFactory->CreateTestDriver("SampleDriverA.dll");
   testDriverHandle->RunTest();
   
   testDriverHandle = testFactory->CreateTestDriver("SampleDriverB.dll");
   testDriverHandle->RunTest();

   return 0;
}

