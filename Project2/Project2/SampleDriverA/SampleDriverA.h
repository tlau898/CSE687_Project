#ifndef SAMPLEDRIVERA_H
#define SAMPLEDRIVERA_H

#ifdef SAMPLEDRIVERA_EXPORTS
   #define DLLEXPORT __declspec(dllexport)
#else
   #define DLLEXPORT __declspec(dllimport)
#endif

#include "../TestDriver/TestDriver.h"

class SampleDriverA : public ITest
{
public:
   bool RunTest();

private:
};


#endif
