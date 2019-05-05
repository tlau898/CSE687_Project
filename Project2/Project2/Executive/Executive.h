#pragma once

#ifndef EXECUTIVE_H
#define EXECUTIVE_H



#include <iomanip>
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <ctime>
#include <list>
#include <iterator>
#include <functional>
#include <sstream>

#include "../TestHarness/TestHarness.h"

using namespace std;

class Executive
{
public:

   void CreateTests(string xmlTestRequest);
   void RunSampleTestSequence();

private:
   TestHarness testHarness;
};

#endif //EXECUTOR_H

