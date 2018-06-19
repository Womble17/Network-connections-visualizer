#include "Reader.hpp"

#include <iostream>

using namespace std;

Reader::Reader(string_queue_ptr& outputQueue) :
    outputQueue_(outputQueue),
    threadRunning_(true),
    internalThread_(&Reader::run, this)
{
    logger_ = "\n[READER] ";
    cout << logger_ << "Constructor";

}

Reader::~Reader()
{
    internalThread_.join();
}

void Reader::run()
{
    cout << logger_ << "Runner";

    int counter = 0;
    string IP;
    while(cin >> IP)
    {
        // filter local communication
        if(IP.find("192.168") != string::npos)
            continue;

        int firstPart = stoi(IP);

        // filter reserved IPs
        if(firstPart >= 224 && firstPart <= 240)
        {
            continue;
        }

        cout << logger_ + to_string(counter++);
        outputQueue_->push(IP);
    }
}

