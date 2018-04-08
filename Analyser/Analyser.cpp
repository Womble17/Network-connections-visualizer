#include "Analyser.hpp"

#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <chrono>
#include <memory>
#include <utility>

using namespace std;

Analyser::Analyser(string_queue_ptr& inputQueue, pair_queue_ptr& outputQueue)
    : inputQueue_(inputQueue)
    , outputQueue_(outputQueue)
    , threadRunning_(true)
    , internalThread_(&Analyser::run, this)
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

    string IP;
    while(threadRunning_)
    {
        if(inputQueue_->pop(IP))
        {
            cout << logger_ + IP;
            analysePacket(IP);
        }

    }
}

void Analyser::analysePacket(string& IP)
{
    cout << __FUNCTION__ << endl;

    auto location = ipLocations.find(IP);

    vector<string> midLocationIPs {IP};//= getMidIPs(IP);

    for(auto& midIP : midLocationIPs)
    {
        if(location == ipLocations.end())
        {
            cout << logger_ << "new connection: " << midIP;
            auto maybeLocation = ipLocations.emplace(midIP, make_shared<IPLocation>(midIP));

            if(maybeLocation.second)
            {
                location = maybeLocation.first;
            }
            else
            {
                cerr << logger_ << "adding new IP location failed";
                return;
            }
        }
        else
        {
            //connections[IP]->ping();
            cout << logger_ << "updating connection: IP: " << midIP;
        }

        if(location->second->geoLongtitude_ && location->second->geoLatitude_)
            outputQueue_->push(make_pair(location->second->geoLongtitude_, location->second->geoLatitude_));
    }

//    if(ipLocations.size() % 4 == 0)
//        outputQueue_->push(make_pair(0.0, 0.0));
}

vector<string> Analyser::getMidIPs(string& IP)
{
    cout << __FUNCTION__ << endl;
    string request = "traceroute " +  IP + " -n | tail -n+2 | awk '{ print $2 }'";
    cout << request << endl;
    FILE* pipe = popen(request.c_str(), "r");

    if (!pipe) return std::vector<string>();

    char buffer[128];
    chrono::system_clock::time_point start = chrono::system_clock::now();

    string tracerouteResult;
    while (!feof(pipe))
    {
        cout << endl <<"wait";
        if(fgets(buffer, 128, pipe) != NULL)
            tracerouteResult += buffer;

        if(chrono::system_clock::now() - start > chrono::milliseconds(500))
        {
            cout << endl <<"break due to timeout" << endl;
            break;
        }
    }

    cout << tracerouteResult << endl;

    vector<string> midIPs;
    boost::split(midIPs, tracerouteResult, [](char c){return c == '\n';});

    vector<string> result;
    for(auto& midIP : midIPs)
    {
        if(midIP != "*" && midIP[0] != (char)0){
            cout << logger_ << "mid IP: " << midIP << endl;
            result.push_back(midIP);
        }
    }

    pclose(pipe);

    return result;
}
