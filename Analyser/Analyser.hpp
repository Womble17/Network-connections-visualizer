#ifndef ANALYSER_HPP
#define ANALYSER_HPP

#include <map>
#include <memory>
#include <string>
#include <thread>

#include <boost/lockfree/spsc_queue.hpp>
#include "Connection.hpp"

using queue_type = boost::lockfree::spsc_queue<int>;
using queue_ptr = std::shared_ptr<queue_type>;

class Analyser
{
public:
    Analyser(queue_ptr& inputQueue, queue_ptr& outputQueue);
    ~Analyser();
    void run();

private:
    queue_ptr inputQueue_;
    queue_ptr outputQueue_;
    bool threadRunning_;
    std::thread internalThread_;
    std::string logger_;
    std::map<std::string, std::shared_ptr<Connection>> connections;

    void analysePacket(std::string);
};

#endif // ANALYSER_HPP
