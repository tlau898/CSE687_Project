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

#include "Node.h"
#include "ExecTester.h"

typedef void (*generic_pointer)();
typedef function<void()> lambda;

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
/*
 * Simple function, printing my name to screen
 * should pass executor function
 */

void printNameF(){
    cout<<"Printing my name: "<<endl;
}
/*
 * Function with out of range exception
 * will be caught by executor function
 */
void outOfRangeF(){
    vector<int> myvect(20);
    myvect.at(21)=20;

}
/*
 * function with bad cast
 * will be caught by executor function
 */
void badCastF(){
    Bar b;
    Foo& f = dynamic_cast<Foo&>(b);
}
/*
 * function that divides by zero
 * will throw an error to be caught by executor funtion
 */
void divideByZeroF(){
    int numer =10;
    int denominator =0;
    if(denominator==0){
        //NOTE: c++ does not have a built-in divideByZero Exception
        throw runtime_error("Math error- tried to divide by 0");
    }

}
/*
 *
 */
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
     * creation of execTester
     */

    ExecTester<generic_pointer > * execTester = new ExecTester<generic_pointer >();

    /*
     * insert function pointers into the execTester list
     */
    generic_pointer g1= printHelloF;
    execTester->insert(g1);

    //function pointer 2
    generic_pointer g2 = printNameF;
    execTester->insert(g2);


    //function pointer 3
    generic_pointer g3 = badCastF;
    execTester->insert(g3);

    //function pointer 4
    generic_pointer g4= outOfRangeF;
    execTester->insert(g4);

    //function pointer 5
    generic_pointer g5 = divideByZeroF;
    execTester->insert(g5);


    //function pointer 6
    generic_pointer g6 = badCastF;
    execTester->insert(g6);




    TestHarness * testHarness1 = new TestHarness();
    testHarness1->executor(*execTester);


    testHarness1->printLevelOneLog();
    cout<<"------------"<<endl;
    testHarness1->printLevelTwoLog();
    cout<<"---------------"<<endl;
    testHarness1->printLevelThreeLog();



    TestHarness * testHarness2 = new TestHarness();

    ExecTester<lambda > * execTester2 = new ExecTester<lambda >();

    execTester2->insert(printHello);

    execTester2->insert(badCast);
    execTester2->insert(divideByZero);
    execTester2->insert(invalidArg);
    execTester2->insert(outOfRange);

    testHarness2->executor(*execTester2);

    cout<<"\n\n---------TESTING LAMBDA's----------"<<endl;

    testHarness2->printLevelOneLog();
    cout<<"------------"<<endl;
    testHarness2->printLevelTwoLog();
    cout<<"---------------"<<endl;
    testHarness2->printLevelThreeLog();







    return 0;
}

