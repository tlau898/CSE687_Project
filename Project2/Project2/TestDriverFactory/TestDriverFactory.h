#ifndef TESTDRIVERFACTORY_H
#define TESTDRIVERFACTORY_H


#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <ctime>
#include <list>
#include <iterator>
#include <functional>
#include <iostream>
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////////
// TestFactory.h - Defines TestFactory class which makes use of singleton design    //
//                 pattern, only one instance of class should ever exist.           //
//                 TestFactory is used to instantiate instances of test drivers.    //
// ver 1.0                                                                          //
// Language:      Visual C++ 2010, SP1                                              //
// Application:   Project 1 CSE 687                                                 //
// Author:        John Schurman, Terence Lau                                        //
//////////////////////////////////////////////////////////////////////////////////////

class TestFactory
{
public:
   void CreateTestDriver(std::string testDriverName);
   static TestFactory* Instance();

private:
   TestFactory(){};
   TestFactory(TestFactory const&) {};
   TestFactory& operator=(TestFactory const&){};
   static TestFactory* p_factInstance;

};

struct ITest
{
   virtual bool RunTest() = 0;
};

class Example : public ITest
{
public:
   bool RunTest();
};

#endif