#ifndef SAMPLEDRIVERB_H
#define SAMPLEDRIVERB_H

#ifdef SAMPLEDRIVERB_EXPORTS
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

#include "../TestDriverFactory/TestDriverFactory.h"

class SampleDriverB : public ITest
{
public:
   bool RunTest();
   ~SampleDriverB();

private:
};


#endif
