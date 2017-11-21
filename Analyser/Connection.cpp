#include "Connection.hpp"

#include <chrono>
#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>

using namespace std;

Connection::Connection(string IP) : IP_(IP)
{
    logger_ = "\n[CONNECTION] ";
    cout << logger_ << "Constructor " << IP;

    findGeolocation();
}

void Connection::addPacket()
{
    timestamps_.push(chrono::system_clock::now());
}

void Connection::updateTimestamps()
{
    auto now = chrono::system_clock::now();

    while(chrono::duration_cast<chrono::milliseconds>(timestamps_.front() - now).count() > 1000)
    {
        timestamps_.pop();
    }
}

void Connection::findGeolocation()
{
    string request = "curl -s http://ip-api.com/line/"+IP_+"?fields=lat,lon";
    FILE* pipe = popen(request.c_str(), "r");

    if (!pipe) return;

    char buffer[128];

    string curlResult;
    while (!feof(pipe))
    {
        if(fgets(buffer, 128, pipe) != NULL)
            curlResult += buffer;
    }

    pclose(pipe);

    vector<string> coordinates;

    if(!curlResult.empty())
    {
        boost::split(coordinates, curlResult, [](char c){return c == '\n';});
        geoLatitude_ = stof(coordinates[0]);
        geoLongtitude_ = stof(coordinates[1]);
    }
    else
    {
        cout << logger_ << "EMPTY COORDINATES";
        geoLatitude_ = 0.0;
        geoLongtitude_ = 0.0;
    }

    cout << endl << geoLatitude_ << " " << geoLongtitude_ << endl;

}
