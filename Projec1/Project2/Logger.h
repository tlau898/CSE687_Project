//

// Created by John Schurman on 4/24/19.

//



#ifndef TESTHARNESS_LOGGER_H

#define TESTHARNESS_LOGGER_H



#include <iostream>

#include <vector>



#include <sstream>

#include <iomanip>

#include <ctime>

using std::vector;

using std::string;

class Logger {

public:

	Logger() = default;



	const vector<string> getLevelOneLog();

	const vector<string> getLevelTwoLog();

	const vector<string> getLevelThreeLog();

	void logTestStatus(int, bool);

	void logTestStatus(int, bool, string, string);

	void clear();







private:

	vector<string>levelOneLog;

	vector<string> levelTwoLog;

	vector<string>levelThreeLog;



	string getCurrDateTime();





};





#endif //TESTHARNESS_LOGGER_H