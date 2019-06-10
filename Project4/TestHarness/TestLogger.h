#ifndef LOGGER_H
#define LOGGER_H

//////////////////////////////////////////////////////////////////////////////////////////
// TestLogger.h - Defines Logger class which contains functions to log the results of   //
//                test runs. Logs are available at three levels of verbosity. Logs      //
//                can also be cleared.                                                   //
// ver 1.0                                                                              //
// Language:      Visual C++ 2010, SP1                                                  //
// Application:   Project 1 CSE 687                                                     //
// Author:        John Schurman, Terence Lau                                            //
//////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

class TestLogger {

public:
   void printLevelOneLog();
   void printLevelTwoLog();
   void printLevelThreeLog();

   void logTestStatus(string testName, bool passed);
   void logTestStatus(string testName, bool passed, string exMsg, string exDetail);
   void clearlog();
   static string getCurrDateTime();

private:
   vector<string>levelOneLog;
   vector<string> levelTwoLog;
   vector<string>levelThreeLog;

};

#endif //LOGGER_H