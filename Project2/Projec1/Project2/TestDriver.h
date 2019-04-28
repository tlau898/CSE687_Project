#pragma once

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

#include "Executive.h"

template <class T>
class TestDriver
{
public:
	TestDriver() = default;
	TestDriver(T&);
	~TestDriver();
	bool ITest(std::string&,std::string&);
private:
	T& functPtr;
	Executive executive;
};

template <class T>
TestDriver<T>::TestDriver(T& func) 
	:functPtr(func){

	//functPtr = func;

}
template<class T>
TestDriver<T>::~TestDriver()
{
}

template<class T>
bool TestDriver<T>::ITest(std::string& msg, std::string&msg2) {
	return executive.executor(msg, msg2,functPtr);
}







#endif //Ending the function

