#include "Executive.h"



Executive::Executive()
{
	testNumber = 0;
}


Executive::~Executive()
{
}
/*
*Calls logger.clear(), which will clear all of the logging vectors
*/
void Executive::clear() {
	logger.clear();

}
/*Gets the level one log from the logger
Will be used in the TestHarness class
*/
std::vector<std::string> Executive::getLevelOneLog() {
	return logger.getLevelOneLog();
}



/*Gets the level tw0 log from the logger
Will be used in the TestHarness class
*/
std::vector<std::string> Executive::getLevelTwoLog() {
	return logger.getLevelTwoLog();
}
/*Gets the level three log from the logger
Will be used in the TestHarness class
*/
std::vector<std::string> Executive::getLevelThreeLog() {
	return logger.getLevelThreeLog();
}
