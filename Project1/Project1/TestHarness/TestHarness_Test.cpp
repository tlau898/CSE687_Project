//////////////////////////////////////////////////////////////////////////////////////
// TestHarness_Test.cpp - Defines several functions, lambda functions to test the   //
//                 TestHarness class. A list of callable objects is passed to the   //
//                 executor method of the TestHarness class. The results of each    //
//                 call to the callable object(s) are then printed at different     //
//                 levels of verbosity.                                             //
// ver 1.0                                                                          //
// Language:      Visual C++ 2010, SP1                                              //
// Application:   Project 1 CSE 687                                                 //
// Author:        John Schurman, Terence Lau                                        //
//////////////////////////////////////////////////////////////////////////////////////
#include<iostream>
using namespace std;

#include "TestHarness.h"
#include <sstream>
#include <functional>
#include <stdexcept>
#include <typeinfo>
#include <vector>
#include <bitset>
#include <ctime>

typedef void (*generic_pointer)();
typedef function<void()> lambda;
/*
 * Simple struct created to demonstrate the ability of functors to work
 * in the executor method of the testHarness class
 * Executor method will catch and document runtime error
 */
struct divideByZeroS{
    int numerator = 10;
    int denominator =0;
    void operator()(){
        if(denominator == 0){
            throw runtime_error("Math error- tried to divide by 0");

        }

    }
};
/*
 * simple struct to demonstrate executor ability to run functors
 * Hello world should print to the screen
 * executor method should pass
 */
struct printHelloS{
    void operator()(){
        cout<<"Hello world"<<endl;
    }

};





/*
 * structs used for demonstrating bad cast error
 * should not be used in any other place except for
 * demonstrating proper catch of error
 */
struct Foo { virtual ~Foo() {} };
struct Bar { virtual ~Bar() {} };

/*
 * struct demonstrating a bad cast example
 * should be caught an documented by executor method
 */

struct badCastS{
    Bar b;

    void operator()(){
        Foo& f = dynamic_cast<Foo&>(b);

    }
};
/*
 * struct used to demonstrated outOFRange error using functors
 * will be caught and documented by the executor method
 */
struct outOfRangeS{
    vector<int> myvect;
    void operator()(){
        myvect.at(50)=30;
    }

};
struct invalidArgS{
    string invalArg = "10012";

    void operator()(){
        std::bitset<5> mybitset(invalArg);
    }

};


/*
 * Simple function, printing hello world to screen
 * should pass executor function
 */
void printHelloF(){
    cout<<"Hello World"<<endl;
}

/*
 * Simple Function with out of range exception
 * will be caught by executor function
 */
void outOfRangeF(){
    vector<int> myvect(20);
    myvect.at(21)=20;
}

/*
 * Simple function with bad cast
 * will be caught by executor function
 */
void badCastF(){
    Bar b;
    Foo& f = dynamic_cast<Foo&>(b);


}

/*
 * Simple function that divides by zero
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
 * Simple function to generate invalid argument
 * The complier will compile because the type of the arugment for bitset is correct
 * Will be invalid because the arugment must contain 1's and 0's
 * Thus, an exception should be caught by executor
*/
void invalidArgF(){
    string invalArg = "10012";
    std::bitset<5> mybitset (invalArg);
}

/*
 * Lambda function to print hello world, should pass executor 
 */
function<void()> printHello=[](){
    cout<<"hello world"<<endl;

};

/*
 * Lambda function to demonstrate the correct handling of divide by 0
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
 * Lambda function to demonstrate the catching of a
 * out_of_range exception
 */
function<void()> outOfRange =[](){
    vector<int> myvect(20);
    myvect.at(21)=20;
};

/*
 * Lambda function demonstrating a bad cast
 * will be caught by executor method
 */
function<void()> badCast=[](){
    Bar b;
    Foo& f = dynamic_cast<Foo&>(b);

};

/*
 * Lambda function for an invalid argument
 *The complier will compile because the type of the arugment for bitset is correct
 * Will be invalid because the arugment must contain 1's and 0's
 * Thus, an exception should be caught by executor
 */
function<void()> invalidArg=[](){
    string invalArg = "10012";
    std::bitset<5> mybitset (invalArg);
};

int main()
{
   cout << "**********************************************" << endl;
   cout << "*   TESTING LIST OF FUNCTION POINTER CALLS   *" << endl;
   cout << "**********************************************" << endl;
   TestHarness testHarness = TestHarness();
   list<generic_pointer> testList;

   /*
   * insert function pointers into the execTester list
   */
   generic_pointer g1 = printHelloF;
   testList.push_back(g1);

   //function pointer 2
   generic_pointer g2 = badCastF;
   testList.push_back(g2);

   //function pointer 3
   generic_pointer g3 = outOfRangeF;
   testList.push_back(g3);

   //function pointer 4
   generic_pointer g4 = divideByZeroF;
   testList.push_back(g4);

   //function pointer 5
   generic_pointer g5 = badCastF;
   testList.push_back(g5);

   //Execute callable objects, print results
   testHarness.executor(testList);
   testHarness.printLevelOneLog();
   testHarness.printLevelTwoLog();
   testHarness.printLevelThreeLog();
   cout << "\n\n\n" << endl;


   cout << "******************************************" << endl;
   cout << "*      TESTING LIST OF LAMBDA CALLS      *" << endl;
   cout << "******************************************" << endl;
   list<lambda> testList2;

   //Add lambda functions to list
   testList2.push_back(badCast);
   testList2.push_back(divideByZero);
   testList2.push_back(invalidArg);
   testList2.push_back(outOfRange);
   testList2.push_back(printHello);

   //Execute callable objects, print results
   testHarness.executor(testList2);
   testHarness.printLevelOneLog();
   testHarness.printLevelTwoLog();
   testHarness.printLevelThreeLog();

    cout << "\n\n\n" << endl;


    cout << "******************************************" << endl;
    cout << "*      TESTING LIST OF FUNCTOR CALLS      *" << endl;
    cout << "******************************************" << endl;

    list<lambda>testList3;
    printHelloS printHelloS1;
    divideByZeroS divideByZeroS1;
    badCastS badCastS1;
    outOfRangeS outOfRangeS1;
    invalidArgS invalidArgS1;

    testList3.emplace_back(printHelloS1);
    testList3.emplace_back(divideByZeroS1);
    testList3.emplace_back(badCastS1);
    testList3.emplace_back(invalidArgS1);
    testList3.emplace_back(outOfRangeS1);


    testHarness.executor(testList3);
    testHarness.printLevelOneLog();
    testHarness.printLevelTwoLog();
    testHarness.printLevelThreeLog();




    return 0;
}

