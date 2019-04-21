////////////////////////////////////////////////////////////////////////////////////////
// TestHarness.cpp - Implements TestHarness class that defines methods to invoke      //
//                   a passed callable object or a list of callable objects. Results  //
//                   are logged and available at three different levels of verbosity. //
// ver 1.0                                                                            //
// Language:      Visual C++ 2010, SP1                                                //
// Application:   Project 1 CSE 687                                                   //
// Author:        John Schurman, Terence Lau                                          //
////////////////////////////////////////////////////////////////////////////////////////

#include "TestHarness.h"

/******************************************************************************************************************
* Function: printLevelOneLog
* Notes:    This function prints the level one log results of an executor call. The level one log includes the
*           test number and the pass/fail result of each callable object.
*
******************************************************************************************************************/
void TestHarness::printLevelOneLog() {
   cout << "********************************" << endl;
   cout << "*    LEVEL ONE STATUS LOG      *" << endl;
   cout << "********************************" << endl;
   for (const std::string &s : levelOneLog) {
      cout << s << endl;
   }
   cout << endl;
}

/******************************************************************************************************************
* Function: printLevelTwoLog
* Notes:    This function prints the level two log results of an executor call. The level two log includes the
*           test number and the pass/fail result of each callable object. If failed, log will also include
*           exception message.
*
******************************************************************************************************************/
void TestHarness::printLevelTwoLog() {
   cout << "********************************" << endl;
   cout << "*    LEVEL TWO STATUS LOG      *" << endl;
   cout << "********************************" << endl;
   for(const std::string &s: levelTwoLog){
      cout<<s<<endl;
   }
   cout << endl;
}

/******************************************************************************************************************
* Function: printLevelThreeLog
* Notes:    This function prints the level three log results of an executor call. The level three log includes the
*           test number and the pass/fail result of each callable object. If failed, log will also include
*           exception message, exception category and timestamp.
*
******************************************************************************************************************/
void TestHarness::printLevelThreeLog() {
   cout << "********************************" << endl;
   cout << "*    LEVEL THREE STATUS LOG    *" << endl;
   cout << "********************************" << endl;
   for (const std::string &s : levelThreeLog) {
      cout << s << endl;
   }
   cout << endl;
}

/******************************************************************************************************************
* Function: logTestStatus (Overloaded)
* Notes:    This function calls an overloaded version of logTestStatus that passes in empty strings for 
*           exception related parameters.
*
******************************************************************************************************************/
void TestHarness::logTestStatus(int testNumber, bool passed)
{
   logTestStatus(testNumber, passed, "", "");
}

/******************************************************************************************************************
* Function: logTestStatus (Overloaded)
* Notes:    This function builds test result logs from the arguments specified, there are three different logs
*           each with increasing verbosity. Level one log contains pass/fail status, level two adds log adds
*           exception message and level three log adds execption category and timestamp.
*
******************************************************************************************************************/
void TestHarness::logTestStatus(int testNumber, bool passed, string exMsg, string exDetail)
{
   std::string dateTime = getCurrDateTime();
   std::stringstream ss;

   ss << left << "TEST ";
   ss << std::to_string(testNumber) << ": ";

   if (passed) 
   {
      ss << setw(10) << "PASSED";
      levelOneLog.emplace_back(ss.str());

      ss << setw(40) << "PASSED";
      levelTwoLog.emplace_back(ss.str());

      ss << dateTime << " ";
      ss << "PASSED";
      levelThreeLog.emplace_back(ss.str());
   }
   else 
   {
      ss << setw(10) << "FAILED";
      levelOneLog.emplace_back(ss.str());

      ss << setw(40) << exMsg;
      levelTwoLog.emplace_back(ss.str());

      ss << dateTime << " ";
      ss << exDetail;
      levelThreeLog.emplace_back(ss.str());
   }
}

/******************************************************************************************************************
* Function: printLevelThreeLog
* Notes:    This function prints the level three log results of an executor call. The level three log includes the
*           test number and the pass/fail result of each callable object. If failed, log will also include
*           exception message, exception category and timestamp.
*
******************************************************************************************************************/
std::string TestHarness::getCurrDateTime() {
   struct tm timeInfo;
   time_t now;
   char buffer[80];

   time(&now);
   localtime_s(&timeInfo,&now);
   strftime(buffer, 80, "[%D-%T]", &timeInfo);

   return buffer;
}

/******************************************************************************************************************
* Function: Copy constructor
* Notes:    The copy constructor copies the values of the test logs
*
******************************************************************************************************************/
TestHarness::TestHarness(const TestHarness &TH) {
   levelOneLog = TH.levelOneLog;
   levelTwoLog= TH.levelTwoLog;
   levelThreeLog = TH.levelThreeLog;

}
