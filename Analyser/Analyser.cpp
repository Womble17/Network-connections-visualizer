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
    cout << logger_ << __FUNCTION__;

    if(notRespondingIP_.count(IP))
    {
        cout << logger_ << "skipping not responding IP";
        return;
    }

    // run traceroute
    vector<string> midLocationIPs = getMidIPs(IP);

    // traceroute query failed, add IP to list of not responding IPs
    if(midLocationIPs.size() < 3)
    {
        setIpAsNotResponding(IP);
        return;
    }

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

    for(unsigned i = 0; i < midLocationIPs.size() - 1; ++i)
    {
        // for each link
        std::pair<string, string> linkNodes = make_pair(midLocationIPs[i], midLocationIPs[i+1]);

        // create new link data if needed
        if(linkDataMap_.count(linkNodes) == 0)
        {
            linkDataMap_[linkNodes] = make_shared<LinkData>();
        }

        // add timestamps
        linkDataMap_[linkNodes]->addPacket();
    }
}

/**
 * @brief Analyser::getMidIPs method running traceroute for given IP address
 * @param IP
 * @return vector of IP adresses returned by traceroute query
 */
vector<string> Analyser::getMidIPs(string& IP)
{
    cout << logger_ << __FUNCTION__;
    string request = "sudo traceroute " +  IP + " -w 0.5 -n -I | tail -n+2 | awk '{ print $2 }'";
    cout << logger_ << request;
    FILE* pipe = popen(request.c_str(), "r");

    if (!pipe) return std::vector<string>();

    char buffer[128];

    string tracerouteResult;
    while (!feof(pipe))
    {
        if(fgets(buffer, 128, pipe) != NULL)
            tracerouteResult += buffer;
    }

    cout << logger_ << "TRACEROUTE RESULTS: for IP: " << IP << ":\n" << tracerouteResult;

    vector<string> midIPs;
    boost::split(midIPs, tracerouteResult, [](char c){return c == '\n';});

    vector<string> result {OWN_IP};

    for(auto& midIP : midIPs)
    {
        if(midIP[0] >= 48 && midIP[0] <= 57)
        {
            cout << logger_ << "mid IP: " << midIP;

            if(IP.find("192.168") != string::npos)
                continue;

            result.push_back(midIP);
        }
        else
        {
            cout << logger_ << "Wrong trace";
        }
    }

    pclose(pipe);

    return result;
}

/**
 * @brief Analyser::findGeolocation method finding geolocation of device with given IP address
 * @param IP
 * @return pair of floats (lattitude and longitude) if API query succeeded, boost::none otherwise
 */
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

    cout << geoLatitude << " " << geoLongtitude;

    // pair x,y
    return make_pair(geoLongtitude, geoLatitude);

}

void Analyser::updateTimestamps()
{
    for(auto& linkData : linkDataMap_)
    {
        linkData.second->updateTimestamps();
    }
}

void Analyser::setIpAsNotResponding(string& IP)
{
    notRespondingIP_.insert(IP);
}

void Analyser::prepareAndSendDataPackage()
{
    shared_ptr<DrawingPackage> packagePtr = make_shared<DrawingPackage>();

    // no link data available - nothing to draw
    if(linkDataMap_.size() == 0)
    {
        return;
    }

    for(auto& linkData : linkDataMap_)
    {
        // all timestamps timed out - not drawing that connection
        if(linkData.second->getAmountOfPackets() == 0)
        {
            continue;
        }

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
    }

    outputQueue_->push(packagePtr);
}
