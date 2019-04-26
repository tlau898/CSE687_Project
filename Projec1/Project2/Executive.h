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
#include "Logger.h"



class Executive
{
public:
	Executive();
	~Executive();

	void clear();


	template <class T>

	bool executor(T&);



	/* Routine to call list of callable objects, logs status of each execution */

	template <class T>

	bool executor(std::list<T>&);

	std::vector<std::string> getLevelOneLog();
	std::vector<std::string> getLevelTwoLog();
	std::vector<std::string> getLevelThreeLog();





private:
	Logger logger;
	int testNumber;
	template <class T>
	bool execute(int testNum,T&);
};


template<class T>

bool Executive::execute(int testNum, T& testCase) {
	try

	{

		//Try to invoke callable object

		testCase();



		//Log success, no exceptions

		//logTestStatus(testNumber, true);

		logger.logTestStatus(testNumber, true);

		return true;

	}



	catch (std::runtime_error & e) {

		//logTestStatus(testNumber, false, e.what(), "RUNTIME_ERROR");



		logger.logTestStatus(testNumber, false, e.what(), "RUNTIME_ERROR");



		return false;

	}



	catch (std::bad_cast & e) {

		//logTestStatus(testNumber, false, e.what(), "BAD_CAST");

		logger.logTestStatus(testNumber, false, e.what(), "BAD_CAST");

		return false;

	}



	catch (std::bad_typeid & e) {

		//logTestStatus(testNumber, false, e.what(), "BAD_TYPEID");





		logger.logTestStatus(testNumber, false, e.what(), "BAD_TYPEID");

		return false;

	}



	catch (std::bad_alloc & e) {

		//logTestStatus(testNumber, false, e.what(), "BAD_ALLOC");



		logger.logTestStatus(testNumber, false, e.what(), "BAD_ALLOC");

		return false;

	}



	catch (std::out_of_range & e) {

		//logTestStatus(testNumber, false, e.what(), "OUT_OF_RANGE");



		logger.logTestStatus(testNumber, false, e.what(), "OUT_OF_RANGE");

		return false;

	}



	catch (std::invalid_argument & e) {

		// logTestStatus(testNumber, false, e.what(), "INVALID_ARGUMENT");



		logger.logTestStatus(testNumber, false, e.what(), "INVALID_ARGUMENT");

		return false;

	}



	catch (std::logic_error & e) {

		//logTestStatus(testNumber, false, e.what(), "LOGIC_ERROR");



		logger.logTestStatus(testNumber, false, e.what(), "LOGIC_ERROR");

		return false;

	}



	catch (std::exception & e) {

		// logTestStatus(testNumber, false, e.what(), "GENERAL_EXCEPTION");



		logger.logTestStatus(testNumber, false, e.what(), "GENERAL_EXCEPTION");

		return false;

	}



	catch (...) {

		//logTestStatus(testNumber, false, "Unknown", "Unknown Exception caught");



		logger.logTestStatus(testNumber, false, "unknown", "Unknown exception caught");

		return false;

	}
}
template <class T>
bool Executive::executor(T& testCase) {
	



	logger.clear();



	//Execute single callable object

	return execute(1, testCase);

}
template <class T>
bool Executive::executor(std::list<T>& testList) {
	bool executorPassing = true;

	int testNumber = 1;

	//Clear logs from previous Executor run



	logger.clear();








	//Iterate over list of callable objects

	for (typename list<T>::iterator i = testList.begin(); i != testList.end(); i++)

	{

		//Execute single callable object

		if (!execute(testNumber, *i)) {

			executorPassing = false;

		}

		testNumber++;

	}



	//If any callable objects fails, return false

	return executorPassing;

}

#endif // !EXECUTIVE_H