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

//    string temp;
    string IP;
    while(threadRunning_)
    {
        if(inputQueue_->pop(IP))
        {
            //IP = to_string(temp);
            cout << logger_ + IP;
            analysePacket(IP);
        }

    }
}

void Analyser::analysePacket(string IP)
{

    if(connections.find(IP) == connections.end()){
        cout << logger_ << "new connection: " << IP;
        connections[IP] = make_shared<Connection>(IP);
    }
    else
    {
        connections[IP]->counter_++;
        cout << logger_ << "updating connection: IP: " << IP << " counter: " << connections[IP]->counter_;
    }
}
