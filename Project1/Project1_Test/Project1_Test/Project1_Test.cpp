// Project1_Test.cpp : Defines the entry point for the console application.
//
#include<iostream>
using namespace std;

#include <sstream>
#include <functional>
#include "TestHarness.h"
#include <stdexcept>
#include <typeinfo>
#include <vector>
#include <bitset>
#include <ctime>
//#include <unistd.h>

/*
 * structs used for demonstrating bad cast error
 * should not be used in any other place except for
 * demonstrating proper catch of error
 */
struct Foo { virtual ~Foo() {} };
struct Bar { virtual ~Bar() {} };

/*
 * function to demonstrate the correct handling of divide by 0
 * would using take numerator and denominator via paramenters
 * does not for demonstration purposes
 */
void divideByZero(){
    int numer =10;
    int denominator =0;
    if(denominator==0){
        //NOTE: c++ does not have a built-in divideByZero Exception
        throw runtime_error("Math error- tried to divide by 0");
    }

}
//returns an integer, and accepts no arguments
/*
 * Simple function, printing hello world to screen
 * should pass executor function
 */
void printHello(){
    cout<<"hello world"<<endl;
}
/*
 * Simple function to demonstrate the catching of a
 * out_of_range exception
 */
void outOfRange(){
    vector<int> myvect(20);
    myvect.at(21)=20;

}
/*
 * Example of an invalid argument
 *The complier will compile because the type of the arugment for bitset is correct
 * Will be invalid because the arugment must contain 1's and 0's
 * Thus, an exception should be caught by
 */
void currentDateTime() {
    struct tm *timeInfo;
    time_t now;
    time(&now);
    char buffer[80];

    timeInfo = localtime(&now);
    strftime(buffer,80,"[%D-%T]",timeInfo);
    puts(buffer);



}


int main()
{
    /*
     * lambda demonstrating a bad cast
     * will be caught by
     */
    function<void()> badCast=[](){
        Bar b;
        Foo& f = dynamic_cast<Foo&>(b);

    };

    function<void()> invalidArg=[](){
        string invalArg = "10012";
        std::bitset<5> mybitset (invalArg);
    };




    TestHarness *t = new TestHarness();
    t->executor(printHello);

    t->executor(divideByZero);
    t->executor(badCast);
    t->executor(outOfRange);

    t->executor(invalidArg);
    t->printLevelOneLog();
    cout<<"------------------------"<<endl;
    t->printLevelTwoLog();
    cout<<"------------------------"<<endl;

    t->printLevelThreeLog();




   return 0;
}

