//

// Created by John Schurman on 4/24/19.

//



#include "Logger.h"



const vector<string> Logger::getLevelOneLog() {

	return levelOneLog;

}



const vector<string> Logger::getLevelTwoLog() {

	return levelTwoLog;

}



const vector<string> Logger::getLevelThreeLog() {

	return levelThreeLog;

}



void Logger::logTestStatus(int testNum, bool passed) {

	//calls other overloaded method for simplicty
	Logger::logTestStatus(testNum, passed, "", "");
}



void Logger::logTestStatus(int testNumber, bool passed, string exMsg, string exDetail) {

	std::string dateTime = getCurrDateTime();
	std::stringstream ss;

	ss << std::left << "TEST ";
	ss << std::to_string(testNumber) << ": ";

	if (passed)
	{
		ss << std::setw(10) << "PASSED";
		levelOneLog.emplace_back(ss.str());

		ss << std::setw(40) << "PASSED";
		levelTwoLog.emplace_back(ss.str());

		ss << dateTime << " ";
		ss << "PASSED";
		levelThreeLog.emplace_back(ss.str());

	}

	else
	{
		ss << std::setw(10) << "FAILED";
		levelOneLog.emplace_back(ss.str());

		ss << std::setw(40) << exMsg;
		levelTwoLog.emplace_back(ss.str());

		ss << dateTime << " ";
		ss << exDetail;
		levelThreeLog.emplace_back(ss.str());
	}
}



string Logger::getCurrDateTime() {

	struct tm timeInfo;
	time_t now;
	char buffer[80];


	time(&now);
	localtime_s(&timeInfo,&now);
	strftime(buffer, 80, "[%D-%T]", &timeInfo);


	//COMMENT LINES BELOW OUT TO RUN PROGRAM

		//UNCOMMENT WHAT I COMMENTED ABOV
	return buffer;

}



void Logger::clear() {

	levelThreeLog.clear();
	levelTwoLog.clear();
	levelOneLog.clear();

}