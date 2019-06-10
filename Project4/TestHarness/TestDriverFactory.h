#ifndef TESTDRIVERFACTORY_H
#define TESTDRIVERFACTORY_H

//////////////////////////////////////////////////////////////////////////////////////
// TestFactory.h - Defines TestFactory class which makes use of singleton design    //
//                 pattern, only one instance of class should ever exist.           //
//                 TestFactory is used to instantiate instances of test drivers.    //
//                 Defines ITest interface to be implemented by every test driver.  //
// ver 1.0                                                                          //
// Language:      Visual C++ 2010, SP1                                              //
// Application:   Project 1 CSE 687                                                 //
// Author:        John Schurman, Terence Lau                                        //
//////////////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <iostream>
#include <Windows.h>

/*********************************************************************************
* Interface: ITest
* Notes:    This interface is meant to be implemented by every test driver class. 
*
**********************************************************************************/
struct ITest
{
public:
   virtual bool RunTest() = 0;   //Test routine defined by test driver
   virtual ~ITest() {};          //Virtual destructor
};

class TestFactory
{
public:
   ITest* CreateTestDriver(std::string testDriverName);    //Creates test driver instance
   static TestFactory* Instance();                         //Handle to TestFactory singleton

private:
   TestFactory(){};                                        //Singleton, block constructor
   TestFactory(TestFactory const&) {};                     //Singleton, block copy
   TestFactory& operator=(TestFactory const&){};           //Singleton, block assignment
   static TestFactory* p_factInstance;                     //Handle to TestFactory singleton

};

#endif