#include "LinkData.hpp"

#include <chrono>
#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>

#define TIMEOUT 10000

using namespace std;

LinkData::LinkData()
{
    logger_ = "\n[LINK_DATA]\n ";
    cout << logger_ << "Constructor ";
}

void LinkData::addPacket()
{
    timestamps_.push_back(chrono::system_clock::now());
}

unsigned LinkData::getAmountOfPackets()
{
    return timestamps_.size();
}

unsigned LinkData::updateTimestamps()
{
    auto now = chrono::system_clock::now();

    while(chrono::duration_cast<chrono::milliseconds>(timestamps_.front() - now).count() > TIMEOUT)
    {
        timestamps_.erase(timestamps_.begin());
    }

    return timestamps_.size();
}
