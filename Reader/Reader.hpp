#ifndef READER_HPP
#define READER_HPP

#include <memory>
#include <string>
#include <thread>

#include <boost/lockfree/spsc_queue.hpp>


using string_queue_type = boost::lockfree::spsc_queue<std::string>;
using string_queue_ptr = std::shared_ptr<string_queue_type>;

class Reader
{
public:
    Reader(string_queue_ptr& outputQueue);
    ~Reader();
    void run();

private:
    string_queue_ptr outputQueue_;
    bool threadRunning_;
    std::thread internalThread_;
    std::string logger_;


};

#endif // READER_HPP
