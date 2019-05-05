#ifndef SAMPLEDRIVERC_H
#define SAMPLEDRIVERC_H

#ifdef SAMPLEDRIVERC_EXPORTS
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

#include "../TestDriverFactory/TestDriverFactory.h"

class SampleDriverC : public ITest
{
public:
   bool RunTest();
   ~SampleDriverC();

private:
};


#endif
