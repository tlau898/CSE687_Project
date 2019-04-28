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


TestHarness::TestHarness(vector<vector< LPCSTR>> v) {
	
	for (int i = 0; i < v.size(); i++) {
		HINSTANCE dll = LoadLibrary(v[i][0]);
		if (dll != NULL) {
			
			dlls.push_back(dll);
			//get the pointer to the function in the DLL
			FARPROC func = GetProcAddress(HMODULE(dll), v[i][1]);
			funcPtr f = (funcPtr)func;

			funcPtr ff = (funcPtr)GetProcAddress(HMODULE(dll), v[i][1]);

			TestDriver<funcPtr> t =  TestDriver<funcPtr>(ff);

			//record function pointers in vectors to be tested later 
			
			functionPointers2.push_back(f);

			
		
			 
		}
		else {
			cout << "DLL was equal to null" << endl;
		}

	}
}


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

	for (const std::string& s : logger.getLevelOneLog()) {

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


/*
void TestHarness::printLevelTwoLog() {

	cout << "********************************" << endl;

	cout << "*    LEVEL TWO STATUS LOG      *" << endl;

	cout << "********************************" << endl;

	for (const std::string& s : logger.getLevelTwoLog()) {

		cout << s << endl;

	}

	cout << endl;

}*/

void TestHarness::printLevelTwoLog() {
	cout << "********************************" << endl;

	cout << "*    LEVEL TWO STATUS LOG      *" << endl;

	cout << "********************************" << endl;

	for (const std::string& s : logger.getLevelTwoLog()) {

		cout << s << endl;

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

	for (const std::string& s : logger.getLevelThreeLog()) {

		cout << s << endl;

	}

	cout << endl;

}



/******************************************************************************************************************

* Function: logTestStatus (Overloaded)



/******************************************************************************************************************

* Function: logTestStatus (Overloaded)

* Notes:    This function builds test result logs from the arguments specified, there are three different logs

*           each with increasing verbosity. Level one log contains pass/fail status, level two adds log adds

*           exception message and level three log adds execption category and timestamp.

*

******************************************************************************************************************/



/*

/******************************************************************************************************************

* Function: printLevelThreeLog

* Notes:    This function prints the level three log results of an executor call. The level three log includes the

*           test number and the pass/fail result of each callable object. If failed, log will also include

*           exception message, exception category and timestamp.

*

******************************************************************************************************************/

/*




   /******************************************************************************************************************

   * Function: Copy constructor

   * Notes:    The copy constructor copies the values of the test logs

   *

   ******************************************************************************************************************/

TestHarness::TestHarness(const TestHarness& TH) {

	//levelOneLog = TH.levelOneLog;

	//levelTwoLog= TH.levelTwoLog;

	//levelThreeLog = TH.levelThreeLog;



	this->logger = TH.logger;





}

bool TestHarness::test() {
	int numTests = 0;
	string msgLevelTwo = "";
	string msgLevelThree = "";



	
	
	
	bool ret = true;

	for (auto td : functionPointers2) {
		
		//bool b = td->ITest(msgLevelTwo, msgLevelThree);
		//bool b = executive.executor(msgLevelTwo, msgLevelThree, td);
		TestDriver<funcPtr> t = TestDriver<funcPtr>(td);
		bool b = t.ITest(msgLevelTwo, msgLevelThree);
		if (!b) {
			logger.logTestStatus(++numTests, b, msgLevelTwo, msgLevelThree);
			ret = false;
		}
		else {
			logger.logTestStatus(++numTests, b);
		}
		
	}
	return ret;
}