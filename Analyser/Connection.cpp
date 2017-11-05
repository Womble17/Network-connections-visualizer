#include "Connection.hpp"

#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>

using namespace std;

Connection::Connection(string IP) : IP_(IP)
{
    logger_ = "\n[CONNECTION] ";
    cout << logger_ << "Constructor " << IP;

    counter_ = 1;

    findGeolocation();
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

    boost::split(coordinates, curlResult, [](char c){return c == '\n';});

    cout << endl << coordinates[0] << " " << coordinates[1] << endl;
    geoLatitude_ = stof(coordinates[0]);
    geoLongtitude_ = stof(coordinates[1]);
}
