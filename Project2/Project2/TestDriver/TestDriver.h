#ifndef TESTDRIVER_H
#define TESTDRIVER_H


#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <ctime>
#include <list>
#include <iterator>
#include <functional>
#include <iostream>

class TestDriver
{
public:
   TestDriver() = default;
   ~TestDriver() = default;

private:

};

struct ITest
{
   virtual bool RunTest() = 0;
};

#endif