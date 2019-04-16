//
// Created by John Schurman on 4/15/19.
//

#ifndef PROJECT1_TEST_TESTHARNESS_H
#define PROJECT1_TEST_TESTHARNESS_H

#include <iostream>
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <ctime>

#include <sstream>
using std::cout;
using std::endl;
using std::vector;
class TestHarness {
private:
    //only thing allocated in class
    //need place to store to free
    std::vector<char*> dateTimes;

    int numTests;
    std::vector<bool> levelOne;
    std::vector<std::string> levelTwo;
    std::vector<std::string> levelThree;

    std::string getCurrDateTime();


    //helper method to fill level two vector
    void fillLevelTwo(bool, const char *);
    //helper method to fill level three vector
    void fillLevelThree(bool,const char *,const std::string);
public:

    TestHarness(){numTests=0;};
    ~TestHarness();
    //return the level one log
    std::vector<bool>getLevelOneLog(){return levelOne;}

    //returns the level two log
    std::vector<std::string>getLevelTwoLog(){return levelTwo;}

    //returns level three log
    std::vector<std::string>getLevelThreeLog(){return levelThree;}

    //prints the level one log
    void printLevelOneLog();

    //prints the level two log
    void printLevelTwoLog();

    //print level three log
    void printLevelThreeLog();

    template <class T>
    bool executor(T&);





};

template<class T>
bool TestHarness::executor(T & c)  {
    try{
        numTests++;
        c();



    }catch (std::runtime_error& e ){


        //std::cout<<"Exception occured: " <<e.what()<<std::endl;
        levelOne.push_back(false);
        //string to insert for level two
        fillLevelTwo(false,e.what());


        fillLevelThree(false, e.what(),"RUNTIME_ERROR");

        return false;
    }
    catch(std::bad_cast&b){


        //std::cout<<b.what()<<endl;
        levelOne.push_back(false);

        //string to insert for level two



        fillLevelTwo(false,b.what());

        fillLevelThree(false, b.what(),"BAD_CAST");

        return false;
    }
    catch(std::out_of_range&o){


        //cout<<o.what()<<endl;
        levelOne.push_back(false);

        //string to insert for level two
        fillLevelTwo(false,o.what());

        fillLevelThree(false, o.what(),"OUT_OF_RANGE");

        return false;
    }
    catch(std::invalid_argument &i){


        // cout<<i.what()<<endl;
        levelOne.push_back(false);

        //string to insert for level two
        fillLevelTwo(false,i.what());


        fillLevelThree(false, i.what(),"INVALID_ARGUMENT");


        return false;
    }
    catch(std::exception& e) {



        //std::cout<< e.what()<<std::endl;
        levelOne.push_back(false);

        //string to insert for level two
        fillLevelTwo(false,e.what());

        fillLevelThree(false, e.what(),"GENERAL_EXCEPTION");

        return false;

    }
    catch(...){
        //cout<<"catching any exception missed"<<endl;
        levelOne.push_back(false);

        //string to insert for level two
        const char* msg = "Unknown Exception caught";
        fillLevelTwo(false,msg);

        return false;
    }
    //std::cout<<"Test was sucessful"<<std::endl;
    levelOne.push_back(true);


    fillLevelTwo(true,"");

    fillLevelThree(true, "","TEST_PASSED");
    return true;

}



#endif //PROJECT1_TEST_TESTHARNESS_H
