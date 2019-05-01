// TestDriver.cpp : Defines the entry point for the console application.
//
#include "TestDriverFactory.h"

TestFactory* TestFactory::p_factInstance = NULL;

TestFactory* TestFactory::Instance()
{
   if (!p_factInstance)
      p_factInstance = new TestFactory;

   return p_factInstance;
}

void TestFactory::CreateTestDriver(std::string testDriverName)
{
   HINSTANCE hDLL = LoadLibraryA(testDriverName.c_str());

   if (!hDLL)
   {
      std::cout << "Failled to load DLL" << std::endl;
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
}

#ifdef TEST_TESTDRIVERFACTORY

int main()
{
   //Test TestDriver here
    return 0;
}

bool Example::RunTest()
{
   return true;
}

#endif

