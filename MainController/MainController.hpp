#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <memory>
#include <string>

#include <QApplication>

#include <boost/lockfree/spsc_queue.hpp>

using string_queue_type = boost::lockfree::spsc_queue<std::string>;
using string_queue_ptr = std::shared_ptr<string_queue_type>;

using pair_queue_type = boost::lockfree::spsc_queue<std::pair<float,float>>;
using pair_queue_ptr = std::shared_ptr<pair_queue_type>;

class MainController
{
public:
    MainController();

    void run(QApplication& app);


private:
    std::string logger_;

};

#endif // MAINCONTROLLER_HPP
