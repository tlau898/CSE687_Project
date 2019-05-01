// SandBox.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <Windows.h>
#include "../TestDriverFactory/TestDriverFactory.h"

using namespace std;

int main()
{
   TestFactory* testFactory = TestFactory::Instance();
   
   testFactory->CreateTestDriver("SampleDriverA.dll");
   testFactory->CreateTestDriver("SampleDriverB.dll");

   return 0;
}

