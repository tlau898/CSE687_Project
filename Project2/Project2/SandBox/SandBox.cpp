// SandBox.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <Windows.h>
#include "../TestDriver/TestDriver.h"

using namespace std;

int main()
{
   HINSTANCE hDLL = LoadLibrary(TEXT("SampleDriverA.dll"));


   if (!hDLL)
   {
      cout << "Failled to laod DLL" << endl;
   }
   else
   {
      FARPROC funct = GetProcAddress((HMODULE)hDLL, "Test");
      funct();
      
      //Here is where we load the create function from each SampleDriver class
      //Each will return an ITest Pointer which is the class object for each SampleDriver
      //Finally we can invoke the RunTest method defined by each SampleDriver

      FARPROC funct2 = GetProcAddress(hDLL, "Create");
      ITest* test = (ITest*)funct2();
      test->RunTest();
   }


   return 0;
}

