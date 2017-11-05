#include "Reader.hpp"

#include <iostream>

using namespace std;

Reader::Reader(queue_ptr& outputQueue) :
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

    string temp;

    while(cin >> temp)
    {
        if(temp.find("192.168") != string::npos)
            continue;

        cout << logger_ + temp;
        outputQueue_->push(temp);
    }
}

