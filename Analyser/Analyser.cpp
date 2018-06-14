#include "Analyser.hpp"

#include <boost/algorithm/string/split.hpp>
#include <boost/optional/optional.hpp>
#include <iostream>
#include <chrono>
#include <memory>
#include <utility>

using namespace std;

Analyser::Analyser(string_queue_ptr& inputQueue, drawings_queue_ptr& outputQueue)
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

    // fill own location
    ipLocations_[OWN_IP] = findGeolocation(OWN_IP).get();

    int counter = 0;
    string IP;
    while(threadRunning_)
    {
        if(inputQueue_->pop(IP))
        {
            cout << logger_ + to_string(counter++);
            analyseNewPacket(IP);
        }
        // for each link update timestamps
        updateTimestamps();
        // create data package
        // send data package
        prepareAndSendDataPackage();
    }
}

void Analyser::analyseNewPacket(string& IP)
{
    cout << __FUNCTION__ << endl;

    // run traceroute
    vector<string> midLocationIPs = getMidIPs(IP);

    for(auto& midIP : midLocationIPs)
    {
        // check for new IPs and find geolocation for them
        if(ipLocations_.count(midIP) == 0)
        {
            auto maybeLocation = findGeolocation(midIP);
            if(maybeLocation)
            {
                ipLocations_[midIP] = maybeLocation.get();
            }
            else
            {
                midLocationIPs.erase(remove(midLocationIPs.begin(), midLocationIPs.end(), midIP));
            }
        }
    }

    if(midLocationIPs.size() < 2)
    {
        return;
    }
    for(unsigned i = 0; i < midLocationIPs.size() - 1; ++i)
    {
        // for each link
        std::pair<string, string> linkNodes = make_pair(midLocationIPs[i], midLocationIPs[i+1]);

        // create new if needed
        if(linkDataMap_.count(linkNodes) == 0)
        {
            linkDataMap_[linkNodes] = make_shared<LinkData>();
        }

        // add timestamps
        linkDataMap_[linkNodes]->addPacket();
    }
}

vector<string> Analyser::getMidIPs(string& IP)
{
    cout << __FUNCTION__ << endl;
    string request = "sudo traceroute " +  IP + " -w 0.05 -n -I | tail -n+2 | awk '{ print $2 }'";
    cout << request << endl;
    FILE* pipe = popen(request.c_str(), "r");

    if (!pipe) return std::vector<string>();

    char buffer[128];

    string tracerouteResult;
    while (!feof(pipe))
    {
        if(fgets(buffer, 128, pipe) != NULL)
            tracerouteResult += buffer;
    }

    cout << "TRACEROUTE RESULTS: for IP: " << IP << ":\n" << tracerouteResult << endl;

    vector<string> midIPs;
    boost::split(midIPs, tracerouteResult, [](char c){return c == '\n';});

    vector<string> result {OWN_IP};

    for(auto& midIP : midIPs)
    {
        if(midIP[0] >= 48 && midIP[0] <= 57)
        {
            cout << logger_ << "mid IP: " << midIP << endl;

            if(IP.find("192.168") != string::npos)
                continue;

            result.push_back(midIP);
        }
        else
        {
            cout << "Wrong trace" << endl;
        }
    }

    pclose(pipe);

    return result;
}

boost::optional< std::pair<float, float> > Analyser::findGeolocation(std::string& IP)
{
    string request = "curl -s http://ip-api.com/line/" + IP + "?fields=lat,lon";
    FILE* pipe = popen(request.c_str(), "r");

    float geoLatitude, geoLongtitude;

    if (!pipe) return boost::none;

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
        geoLatitude = stof(coordinates[0]);
        geoLongtitude = stof(coordinates[1]);
    }
    else
    {
        cout << logger_ << "EMPTY COORDINATES";

        return boost::none;
    }

    cout << endl << geoLatitude << " " << geoLongtitude << endl;

    // pair x,y
    return make_pair(geoLongtitude, geoLatitude);

}

void Analyser::updateTimestamps()
{
    unsigned temp_throughput;
    for(auto& linkData : linkDataMap_)
    {
//        temp_throughput = linkData.second->updateTimestamps();

//        if(temp_throughput > max_throughput)
//        {
//            max_throughput = temp_throughput;
//        }
        linkData.second->updateTimestamps();
    }
    // todo delete old timestamps
}

void Analyser::prepareAndSendDataPackage()
{
    shared_ptr<DrawingPackage> packagePtr = make_shared<DrawingPackage>();

    if(linkDataMap_.size() == 0)
    {
        return;
    }

    for(auto& linkData : linkDataMap_)
    {
        CurveData curveData;
        // both x and y from iplocation map
        curveData.x1 = ipLocations_[linkData.first.first].first;
        curveData.y1 = ipLocations_[linkData.first.first].second;

        curveData.x2 = ipLocations_[linkData.first.second].first;
        curveData.y2 = ipLocations_[linkData.first.second].second;

        // rgb based on timestamps - for now only count, later on their average value also
        if(linkData.second->getAmountOfPackets() > 0.7 * max_throughput)
        {
            curveData.r = 255;
        }
        else if(linkData.second->getAmountOfPackets() > 0.4 * max_throughput)
        {
            curveData.g = 255;
        }
        else
        {
            curveData.b = 255;
        }

        packagePtr->drawDataPackage.push_back(curveData);
        cout << "curve data added to package" << endl;
    }

    outputQueue_->push(packagePtr);
}
