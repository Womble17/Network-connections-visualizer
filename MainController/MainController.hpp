#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <memory>
#include <string>

#include <boost/lockfree/spsc_queue.hpp>

using queue_type = boost::lockfree::spsc_queue<int>;
using queue_ptr = std::shared_ptr<queue_type>;

class MainController
{
public:
    MainController();

    void run();


private:
    std::string logger_;

};

#endif // MAINCONTROLLER_HPP
