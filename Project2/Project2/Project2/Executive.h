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

	template<class T>
	bool executor(std::string&,std::string&,T&);







private:
	Logger logger;
	int testNumber;
	template <class T>
	bool execute(int testNum,T&);
};

template<class T>
bool Executive::executor(std::string& levelTwoMessage,std::string& levelThreeMessage, T& testCase) {
	try

	{

		//Try to invoke callable object

		testCase();



		//Log success, no exceptions

		//logTestStatus(testNumber, true);

		logger.logTestStatus(testNumber, true);

		levelTwoMessage = "The testcase passed";
		levelThreeMessage = "";

		return true;

	}



	catch (std::runtime_error & e) {

		//logTestStatus(testNumber, false, e.what(), "RUNTIME_ERROR");



		logger.logTestStatus(testNumber, false, e.what(), "RUNTIME_ERROR");

		levelTwoMessage = e.what();
		levelThreeMessage = "RUNTIME_ERROR";



		return false;

	}



	catch (std::bad_cast & e) {

		//logTestStatus(testNumber, false, e.what(), "BAD_CAST");

		logger.logTestStatus(testNumber, false, e.what(), "BAD_CAST");
		levelTwoMessage = e.what();
		levelThreeMessage = "BAD_CAST";



		return false;

	}



	catch (std::bad_typeid & e) {

		//logTestStatus(testNumber, false, e.what(), "BAD_TYPEID");





		logger.logTestStatus(testNumber, false, e.what(), "BAD_TYPEID");
		levelTwoMessage = e.what();
		levelThreeMessage = "BAD_TYPEID";

		return false;

	}



	catch (std::bad_alloc & e) {

		//logTestStatus(testNumber, false, e.what(), "BAD_ALLOC");



		logger.logTestStatus(testNumber, false, e.what(), "BAD_ALLOC");
		levelTwoMessage = e.what();
		levelThreeMessage = "BAD_ALLOC";

		return false;

	}



	catch (std::out_of_range & e) {

		//logTestStatus(testNumber, false, e.what(), "OUT_OF_RANGE");



		logger.logTestStatus(testNumber, false, e.what(), "OUT_OF_RANGE");
		levelTwoMessage = e.what();

		levelThreeMessage = "OUT_OF_RANGE";

		return false;

	}



	catch (std::invalid_argument & e) {

		// logTestStatus(testNumber, false, e.what(), "INVALID_ARGUMENT");



		logger.logTestStatus(testNumber, false, e.what(), "INVALID_ARGUMENT");
		levelTwoMessage = e.what();

		levelThreeMessage = "INVALID_ARGUMENT";

		return false;

	}



	catch (std::logic_error & e) {

		//logTestStatus(testNumber, false, e.what(), "LOGIC_ERROR");



		logger.logTestStatus(testNumber, false, e.what(), "LOGIC_ERROR");
		levelTwoMessage = e.what();
		levelThreeMessage = "LOGIC_ERROR";

		return false;

	}



	catch (std::exception & e) {

		// logTestStatus(testNumber, false, e.what(), "GENERAL_EXCEPTION");



		logger.logTestStatus(testNumber, false, e.what(), "GENERAL_EXCEPTION");
		levelTwoMessage = e.what();

		levelThreeMessage = "GENERAL_EXCEPTION";

		return false;

	}



	catch (...) {

		//logTestStatus(testNumber, false, "Unknown", "Unknown Exception caught");



		logger.logTestStatus(testNumber, false, "unknown", "Unknown exception caught");
		levelTwoMessage = "Unknown";

		levelThreeMessage = "unknown exception caught";

		return false;

	}

}

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
	std::cout << "Called correct executor function" << std::endl;

	std::cout << testList.size() << std::endl;








	//Iterate over list of callable objects

	for (typename list<T>::iterator i = testList.begin(); i != testList.end(); i++)

	{
		std::cout << "Got here2" << std::endl;

		//Execute single callable object

		if (!execute(testNumber, *i)) {
			std::cout << "Got here2" << std::endl;

			executorPassing = false;

		}

		testNumber++;

	}



	//If any callable objects fails, return false

	return executorPassing;

}

#endif // !EXECUTIVE_H