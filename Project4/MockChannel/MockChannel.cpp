/////////////////////////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Defines Mock Channel class which provides ability to send messages from   //
//                   client GUI to the TestHarness using message queues. This class is derived //
//                   from Professor Fawcett's implementation of Mock Channel.                  //
// ver 1.0                                                                                     //
// Language:      Visual C++ 2010, SP1                                                         //
// Application:   Project 1 CSE 687                                                            //
// Author:        John Schurman, Terence Lau                                                   //
/////////////////////////////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "MockChannel.h"
#include "Cpp11-BlockingQueue.h"
#include <string>
#include <thread>
#include <iostream>
#include "../TestHarness/TestHarness.h"
#include "../Executive/Client.h"

using BQueue = BlockingQueue < Msg >;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
  void postMessage(const Msg& msg);
  BQueue& queue();
private:
  BQueue sendQ_;
};

void Sendr::postMessage(const Msg& msg)
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
   Msg getMessage();
  BQueue& queue();
private:
  BQueue recvQ_;
};

Msg Recvr::getMessage()
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
  void start();
  void stop();
  BlockingQueue<string>& queue();

private:
  std::thread thread_;
  std::thread threadTReq_;
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  TestHarness tHarness;
  Client clientObj;
  SocketSystem ss;
  bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr), tHarness(5), clientObj(&tHarness) {}

BlockingQueue<string>& MockChannel::queue()
{
   return clientObj.MsgQueue;
}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
   tHarness.start();
   clientObj.Start();

  std::cout << "\n  MockChannel starting up";

  //This thread receives messages for the GUI
  thread_ = std::thread(
    [this] {
    Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
    Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
    if (pSendr == nullptr || pRecvr == nullptr)
    {
      std::cout << "\n  failed to start Mock Channel\n\n";
      return;
    }
    BQueue& sendQ = pSendr->queue();
    BQueue& recvQ = pRecvr->queue();
    while (!stop_)
    {
       recvQ.enQ(queue().deQ());
    }
    std::cout << "\n  Server stopping\n\n";
  });

  //This thread sends messages back to testHarness based on GUI selection
  threadTReq_ = std::thread(
     [this] {
     Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
     Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
     if (pSendr == nullptr || pRecvr == nullptr)
     {
        std::cout << "\n  failed to start Mock Channel\n\n";
        return;
     }
     BQueue& sendQ = pSendr->queue();
     BQueue& recvQ = pRecvr->queue();
     while (!stop_)
     {
        clientObj.SendRequest(sendQ.deQ());
     }
     std::cout << "\n  Server stopping\n\n";
  });


}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr) 
{ 
  return new MockChannel(pISendr, pIRecvr); 
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
  Msg msg = pRecvr->getMessage();
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
