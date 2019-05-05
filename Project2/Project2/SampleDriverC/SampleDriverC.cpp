// SampleDriverB.cpp : Defines the exported functions for the DLL application.
//

#include "SampleDriverC.h"

using namespace std;

struct Foo { virtual ~Foo() {} };
struct Bar { virtual ~Bar() {} };

void _stdcall badCastF() 
{
   Bar b;
   Foo& f = dynamic_cast<Foo&>(b);
}

bool SampleDriverC::RunTest()
{
   cout << "SampleDriverC test routine invoked" << endl;
   badCastF();

   //No exceptions, test passed
   return true;
}

SampleDriverC::~SampleDriverC()
{
   cout << "SampleDriverC Destructor was invoked" << endl;
}

DLLEXPORT ITest* Create()
{
   ITest* testDriverA = new SampleDriverC();
   return testDriverA;
}



