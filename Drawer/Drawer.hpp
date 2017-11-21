#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <memory>
#include <string>
#include <QApplication>

#include <Map/Map.hpp>

#include <boost/lockfree/spsc_queue.hpp>

using pair_queue_type = boost::lockfree::spsc_queue<std::pair<float,float>>;
using pair_queue_ptr = std::shared_ptr<pair_queue_type>;

class Drawer
{
public:
    Drawer(pair_queue_ptr& inputQueue, QApplication& app);
    ~Drawer();

    void run(QApplication& app);

private:
    pair_queue_ptr inputQueue_;
    bool threadRunning_;
    std::string logger_;
};

#endif // DRAWER_HPP
