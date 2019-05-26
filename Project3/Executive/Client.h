#ifndef CLIENT_H
#define CLIENT_H
////////////////////////////////////////////////////////////////////////////////////////
// Client.h - Implements Client class that defines methods to post test requests to   //
//            a TestHarness object.                                                   //
// ver 1.0                                                                            //
// Language:      Visual C++ 2010, SP1                                                //
// Application:   Project 1 CSE 687                                                   //
// Author:        Terence Lau, John Schurman                                          //
////////////////////////////////////////////////////////////////////////////////////////

#include "../TestHarness/TestHarness.h"

class Client {

public:
   Client(TestHarness* testHarness);        //Constructor
   void AddTests(string xmlTestRequest);    //Post test requests

private:
   TestHarness* tHarness;

};

#endif
