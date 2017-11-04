#ifndef READER_HPP
#define READER_HPP

#include <memory>
#include <string>
#include <thread>

#include <boost/lockfree/spsc_queue.hpp>


using queue_type = boost::lockfree::spsc_queue<int>;
using queue_ptr = std::shared_ptr<queue_type>;

class Reader
{
public:
    Reader(queue_ptr& outputQueue);
    ~Reader();
    void printstuff();
    void run();

private:
    queue_ptr outputQueue_;
    bool threadRunning_;
    std::thread internalThread_;
    std::string logger_;


};

#endif // READER_HPP
