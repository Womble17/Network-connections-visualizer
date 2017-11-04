#include "Analyser.hpp"

#include <iostream>
#include <memory>
#include <utility>

using namespace std;

Analyser::Analyser(queue_ptr& inputQueue, queue_ptr& outputQueue) :
    inputQueue_(inputQueue),
    outputQueue_(outputQueue),
    threadRunning_(true),
    internalThread_(&Analyser::run, this)
{
    logger_ = "\n[ANALYSER] ";
    cout << logger_ << "Constructor ";

}

Analyser::~Analyser()
{
    internalThread_.join();
}

void Analyser::run()
{
    cout << logger_ << "Runner";

    int temp;
    string IP;
    while(threadRunning_)
    {
        if(inputQueue_->pop(temp))
        {
            IP = to_string(temp);
            cout << logger_ + IP;
            analysePacket(IP);
        }

    }
}

void Analyser::analysePacket(string IP)
{

    if(connections.find(IP) != connections.end()){
        // new connection to add
    }
    else
    {
        // connection already in map - updating
    }
}
