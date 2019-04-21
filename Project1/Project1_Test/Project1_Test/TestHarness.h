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
#include "ExecTester.h"
#include "Node.h"
using std::cout;
using std::endl;
using std::vector;
class TestHarness {
private:
    //only thing allocated in class
    //need place to store to free
    std::vector<char*> dateTimes;

    //indicates the number of tests that have been done
    //increments every time a function is error-checked
    int numTests;
    //level one vector that holds success of each test
    std::vector<bool> levelOne;

    //documents the success of each test, as well as the error message
    std::vector<std::string> levelTwo;

    //documents the success of each test, as well as a time stamp and error message
    std::vector<std::string> levelThree;

    /*
     * helper function that returns the current date and time in the following format:
     * M/D/Y-H:M:S
     */
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


    /*
     * overloaded executor function that only executes a single function
     * returns true if the function is sucessful and false it is is not
     */

    template <class T>
    bool executor(T&);

    /*
     * Overloaded exectuor function,
     * takes a Template ExecutorTester function
     * returns tru
     */


    template <class T>
    bool executor(ExecTester<T> &);











};
template <class T>
bool TestHarness::executor(ExecTester<T> & list) {
    bool foundError = false;
    Node<T>* node = list.getHead();
    while(node!= nullptr){
        bool errorWithNode = false;
        try{
            numTests++;

            node->data();
        }catch (std::runtime_error& e ){


            //std::cout<<"Exception occured: " <<e.what()<<std::endl;
            levelOne.push_back(false);
            //string to insert for level two
            fillLevelTwo(false,e.what());


            fillLevelThree(false, e.what(),"RUNTIME_ERROR");

            foundError=true;
            errorWithNode=true;

        }
        catch(std::exception& e) {



            //std::cout<< e.what()<<std::endl;
            levelOne.push_back(false);

            //string to insert for level two
            fillLevelTwo(false,e.what());

            fillLevelThree(false, e.what(),"GENERAL_EXCEPTION");


            foundError=true;
            errorWithNode=true;

        }
        catch(...){
            //cout<<"catching any exception missed"<<endl;
            levelOne.push_back(false);

            //string to insert for level two
            const char* msg = "Unknown Exception caught";
            fillLevelTwo(false,msg);

            foundError=true;
            errorWithNode=true;
        }
        if(!errorWithNode){
            levelOne.push_back(true);


            fillLevelTwo(true,"");

            fillLevelThree(true, "","TEST_PASSED");
        }

        node=node->next;





    }
    return !foundError;
}

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
