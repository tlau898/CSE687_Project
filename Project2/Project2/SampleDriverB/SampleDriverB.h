#ifndef SAMPLEDRIVERA_H
#define SAMPLEDRIVERA_H

#ifdef SAMPLEDRIVERB_EXPORTS
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

#include "../TestDriver/TestDriver.h"

class SampleDriverB : public ITest
{
public:
   bool RunTest();

private:
};


#endif
