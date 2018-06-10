#ifndef ANALYSER_HPP
#define ANALYSER_HPP

#include <map>
#include <memory>
#include <string>
#include <thread>

#include <boost/lockfree/spsc_queue.hpp>
#include <boost/optional/optional.hpp>
#include <Protocols/Aliases.hpp>
#include <Protocols/DrawingPackage.hpp>

#include "LinkData.hpp"

//using link = std::pair<std::string, std::string>;

class Analyser
{
public:
    Analyser(string_queue_ptr& inputQueue, drawings_queue_ptr& outputQueue);
    ~Analyser();
    void run();

private:
    string_queue_ptr inputQueue_;
    drawings_queue_ptr outputQueue_;
    bool threadRunning_;
    std::thread internalThread_;
    std::string logger_;
    std::map< std::pair<std::string, std::string>, std::shared_ptr<LinkData> > linkDataMap_;
    std::map< std::string, std::pair<float, float> > ipLocations_;

    unsigned max_throughput = 1;

    void analyseNewPacket(std::string&);
    void updateTimestamps();
    void prepareAndSendDataPackage();

    boost::optional<std::pair<float, float> > findGeolocation(std::string&);
    std::vector<std::string> getMidIPs(std::string&);

    std::string OWN_IP = "79.110.192.246";
};

#endif // ANALYSER_HPP
