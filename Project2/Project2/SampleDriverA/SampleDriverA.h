#ifndef SAMPLEDRIVERA_H
#define SAMPLEDRIVERA_H

#ifdef SAMPLEDRIVERA_EXPORTS
   #define DLLEXPORT __declspec(dllexport)
#else
   #define DLLEXPORT __declspec(dllimport)
#endif

#include "../TestDriverFactory/TestDriverFactory.h"

class SampleDriverA : public ITest
{
public:
   bool RunTest();
   ~SampleDriverA();

private:
};


#endif
