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

typedef function<void()&> lam;

/*
 * structs used for demonstrating bad cast error
 * should not be used in any other place except for
 * demonstrating proper catch of error
 */
struct Foo { virtual ~Foo() {} };
struct Bar { virtual ~Bar() {} };



/*
 * Simple function, printing hello world to screen
 * should pass executor function
 */

void printHelloF(){
    cout<<"Hello World"<<endl;
}
void outOfRangeF(){
    vector<int> myvect(20);
    myvect.at(21)=20;

}
void badCastF(){
    Bar b;
    Foo& f = dynamic_cast<Foo&>(b);
}
void divideByZeroF(){
    int numer =10;
    int denominator =0;
    if(denominator==0){
        //NOTE: c++ does not have a built-in divideByZero Exception
        throw runtime_error("Math error- tried to divide by 0");
    }

}
void invalidArgF(){
    string invalArg = "10012";
    std::bitset<5> mybitset (invalArg);
}
function<void()> printHello=[](){
    cout<<"hello world"<<endl;

};



/*
 * function to demonstrate the correct handling of divide by 0
 * would using take numerator and denominator via paramenters
 * does not for demonstration purposes
 */

function<void()> divideByZero=[](){
    int numer =10;
    int denominator =0;
    if(denominator==0){
        //NOTE: c++ does not have a built-in divideByZero Exception
        throw runtime_error("Math error- tried to divide by 0");
    }

};
/*
 * Simple function to demonstrate the catching of a
 * out_of_range exception
 */
 function<void()> outOfRange =[](){
    vector<int> myvect(20);
    myvect.at(21)=20;
};
/*
    * lambda demonstrating a bad cast
    * will be caught by executor method
    */

function<void()> badCast=[](){
    Bar b;
    Foo& f = dynamic_cast<Foo&>(b);

};

/*
 * Example of an invalid argument
 *The complier will compile because the type of the arugment for bitset is correct
 * Will be invalid because the arugment must contain 1's and 0's
 * Thus, an exception should be caught by
 */
function<void()> invalidArg=[](){
    string invalArg = "10012";
    std::bitset<5> mybitset (invalArg);
};

int main()
{
    /*
     * Create a fill a vector of lambdas to demonstrate ability to test each
     */

    vector<function<void()>> vector1;

    vector1.push_back(printHello);
    vector1.push_back(outOfRange);
    vector1.push_back(invalidArg);
    vector1.push_back(badCast);
    vector1.push_back(divideByZero);

    /*
    * Create a fill a vector of function pointers to demonstrate ability to test each
    */

    vector<void(*)()> vector2;
    void (*fcnPtr)() = printHelloF;
    vector2.push_back(fcnPtr);

   fcnPtr = outOfRangeF;
   vector2.push_back(fcnPtr);

    fcnPtr = invalidArgF;
    vector2.push_back(fcnPtr);

    fcnPtr = badCastF;
    vector2.push_back(fcnPtr);

    fcnPtr = divideByZeroF;
    vector2.push_back(fcnPtr);











    TestHarness * testHarness1 = new TestHarness();

    for(int i=0;i<vector1.size();i++){
        testHarness1->executor(vector1[i]);
    }

    testHarness1->printLevelOneLog();
    cout<<"----------------------------"<<endl;
    testHarness1->printLevelTwoLog();
    cout<<"----------------------------"<<endl;
    testHarness1->printLevelThreeLog();




    TestHarness * testHarness2 = new TestHarness();
    cout<<"----------------------------"<<endl;
    cout<<"Demonstrating the ability to loop through function pointers"<<endl;




    for(int i=0;i<vector2.size();i++){
        testHarness2->executor(vector2[i]);
    }


    testHarness2->printLevelOneLog();
    cout<<"----------------------------"<<endl;
    testHarness2->printLevelTwoLog();
    cout<<"----------------------------"<<endl;
    testHarness2->printLevelThreeLog();

















    /*
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

    t->printLevelThreeLog();*/




   return 0;
}

