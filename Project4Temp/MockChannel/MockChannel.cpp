/////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Demo for CSE687 Project #4, Spring 2015               //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
//                                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015               //
/////////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "MockChannel.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.cpp"
#include "Cpp11-BlockingQueue.h"
#include <string>
#include <thread>
#include <iostream>
#include "../Executive/Client.h"
#include "../Executive/Client.cpp"
#include "../TestHarness/TestHarness.h"
#include "../TestHarness/TestHarness.cpp"
#include "../TestHarness/TestLogger.h"
#include "../TestHarness/TestLogger.cpp"
#include "../MsgPassingComm/Comm.h"

#include "../MsgPassingComm/Comm.cpp"
#include "../Sockets/Sockets.h"
#include "../Sockets/Sockets.cpp"
#include "../Message/Message.h"
#include "../Message/Message.cpp"
#include "../Utilities/Utilities.h"
#include "../Utilities/Utilities.cpp"
#include "../TestHarness/TestDriverFactory.h"
#include "../TestHarness/TestDriverFactory.cpp"


using BQueue = BlockingQueue < std::string >;
using cln = Client;
SocketSystem ss;

//Will have to change the name of testharness eventually 
class Clint : public iClient {
public:
	void addTest(const std::string& msg);
	void start();


	
private:
//	Client client;
	TestHarness testharness;
};

void Clint::addTest(const std::string& msg) {
	testharness.addTests(msg);


}
void Clint::start() {
	testharness.start();
}

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
  void postMessage(const std::string& msg);
  BQueue& queue();

private:
  BQueue sendQ_;

 
};


void Sendr::postMessage(const std::string& msg)
{
  sendQ_.enQ(msg);
}

BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
	std::string getMessage();
  BQueue& queue();

private:
  BQueue recvQ_;

};

std::string Recvr::getMessage()
{
  return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
  return recvQ_;
}
/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
  MockChannel(ISendr* pSendr, IRecvr* pRecvr);
  MockChannel(ISendr* pSendr, IRecvr* pRecvr, iClient* testHarness);
  void start();
  void stop();
private:
  std::thread thread_;
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  iClient* pIclient_;
  bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}
MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr, iClient* testHarness)
	: pISendr_(pSendr), pIRecvr_(pRecvr), pIclient_(testHarness) {}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
  std::cout << "\n  MockChannel starting up";
  thread_ = std::thread(
    [this] {
    Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
    Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
	Clint* pTestHarness = dynamic_cast<Clint*>(pIclient_);
    if (pSendr == nullptr || pRecvr == nullptr || pTestHarness == nullptr)
    {
      std::cout << "\n  failed to start Mock Channel\n\n";
      return;
    }
    BQueue& sendQ = pSendr->queue();
    BQueue& recvQ = pRecvr->queue();

	//Start the testHarness
	pTestHarness->start();
	//Client* cln = pSendr->createClient();
	std::string startDelimiter = "<testelement>"; //XML
	std::string stopDelimiter = "</testelement>";

	
    while (!stop_)
    {
		
      std::cout << "\n  channel deQing message";
	  const std::string msg = sendQ.deQ();// will block here so send quit message when stopping
	  std::string testRequest = startDelimiter + msg + stopDelimiter;

	  pTestHarness->addTest(testRequest);
	
	  //cln->AddTests(msg);
      std::cout << "\n  channel enQing message";
      recvQ.enQ(msg);
    }
    std::cout << "\n  Server stopping\n\n";
  });
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

iClient* ObjectFactory::createClient() { return new Clint; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr, iClient* testHarness) 
{ 
  return new MockChannel(pISendr, pIRecvr, testHarness); 
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
