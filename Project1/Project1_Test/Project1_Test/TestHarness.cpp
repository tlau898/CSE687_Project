//
// Created by John Schurman on 4/15/19.
//
#include "TestHarness.h"


void TestHarness::printLevelOneLog() {
    int i=1;
    for(bool b : levelOne){
        cout<<"Test "<<i++ <<": ";
        if(b){
            cout<<"pass";
        }else{
            cout<<"fail";
        }
        cout<<endl;

    }

}
void TestHarness::printLevelTwoLog() {

    for(const std::string &s: levelTwo){
        cout<<s<<endl;
    }



}
std::string TestHarness::getCurrDateTime() {
    struct tm *timeInfo;
    time_t now;
    time(&now);
    char *  buffer;
    buffer = new char[80];



    timeInfo = localtime(&now);
    strftime(buffer,80,"[%D-%T]",timeInfo);

    return buffer;
}




void TestHarness::fillLevelTwo(bool err, const char * msg) {
    std::stringstream ss;
    ss<< "Test ";
    ss<< std::to_string(numTests) <<": ";
    if(err){

        ss<<"pass";

    }else{
        std::string s = msg;


        ss<< s;

    }

    levelTwo.emplace_back(ss.str());

}

TestHarness::~TestHarness() {
    for(int i=0;i<dateTimes.size();i++){
        free(dateTimes.at(i));
    }

}

void TestHarness::fillLevelThree(bool err, const char * msg,const std::string error) {
    std::string dateTime = getCurrDateTime();
    std::stringstream ss;
    ss<<dateTime;
   // if(!err) {
        ss << " - ";
        ss << error;
    //}

    if(!err){
        ss<< " - ";
        ss<<msg;
    }



    levelThree.emplace_back(ss.str());

}

void TestHarness::printLevelThreeLog() {
    for(const std::string &s: levelThree){
        cout<<s<<endl;
    }

}
