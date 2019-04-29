#ifndef TESTHARNESS_H

#define TESTHARNESS_H

//////////////////////////////////////////////////////////////////////////////////////

// TestHarness.h - Implements TestHarness class that defines methods to invoke      //

//                 a passed callable object or a list of callable objects. Results  //

//                 are logged and available at three different levels of verbosity. //

// ver 1.0                                                                          //

// Language:      Visual C++ 2010, SP1                                              //

// Application:   Project 1 CSE 687                                                 //

// Author:        John Schurman, Terence Lau                                        //

//////////////////////////////////////////////////////////////////////////////////////



#include <iostream>

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
#include <Windows.h>
#include <windows.h>


#include "Logger.h"
#include "Executive.h"
#include "TestDriver.h"


using namespace std;
typedef void (__stdcall* funcPtr)();

class TestHarness {



public:

	TestHarness() = default;

	~TestHarness() = default;
	TestHarness(vector<vector<LPCSTR>>);

	TestHarness(const TestHarness&);



	/* Routine to execute callable object and perform, logs status of execution */

	//template <class T>

	//bool executor(T&);



	/* Routine to call list of callable objects, logs status of each execution */

	//template <class T>

	//bool executor(list<T>&);

	bool test();


	void printLevelOneLog();

	void printLevelTwoLog();

	void printLevelThreeLog();


	//template <class T>
	//bool test(std::list<T>&);

	//template<class T>
	//bool test(T&);





private:
	//Executive executive;

	


	
	Logger logger;
	vector<HINSTANCE>dlls;


	list<funcPtr> functionPointers2;
	


	



};





#endif //TEST_TESTHARNESS_H
