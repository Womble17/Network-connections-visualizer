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

void Reader::printstuff()
{
    cout << "dupa" << endl;
}

void Reader::run()
{
    cout << logger_ << "Runner";

    int temp;

    while(cin >> temp)
    {
        cout << logger_ + to_string(temp);
        outputQueue_->push(temp);
    }
}

