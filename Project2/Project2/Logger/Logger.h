#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

class Logger {

public:
   const vector<string> getLevelOneLog();
   const vector<string> getLevelTwoLog();
   const vector<string> getLevelThreeLog();

   void printLevelOneLog();
   void printLevelTwoLog();
   void printLevelThreeLog();

   void logTestStatus(int, bool);
   void logTestStatus(int, bool, string, string);
   void clearlog();

private:
   vector<string>levelOneLog;
   vector<string> levelTwoLog;
   vector<string>levelThreeLog;
   string getCurrDateTime();
};

#endif //LOGGER_H