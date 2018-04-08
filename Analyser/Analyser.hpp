#ifndef ANALYSER_HPP
#define ANALYSER_HPP

#include <map>
#include <memory>
#include <string>
#include <thread>

#include <boost/lockfree/spsc_queue.hpp>
#include "IPLocation.hpp"

using string_queue_type = boost::lockfree::spsc_queue<std::string>;
using string_queue_ptr = std::shared_ptr<string_queue_type>;

using pair_queue_type = boost::lockfree::spsc_queue<std::pair<float,float>>;
using pair_queue_ptr = std::shared_ptr<pair_queue_type>;

class Analyser
{
public:
    Analyser(string_queue_ptr& inputQueue, pair_queue_ptr& outputQueue);
    ~Analyser();
    void run();

private:
    string_queue_ptr inputQueue_;
    pair_queue_ptr outputQueue_;
    bool threadRunning_;
    std::thread internalThread_;
    std::string logger_;
    std::map<std::string, std::shared_ptr<IPLocation>> ipLocations;

    void analysePacket(std::string&);
    std::vector<std::string> getMidIPs(std::string&);

};

#endif // ANALYSER_HPP
