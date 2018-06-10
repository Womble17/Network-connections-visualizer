#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <memory>
#include <string>
#include <QApplication>

#include <boost/lockfree/spsc_queue.hpp>

#include <Map/Map.hpp>
#include <Protocols/Aliases.hpp>
#include <Protocols/DrawingPackage.hpp>

class Drawer
{
public:
    Drawer(drawings_queue_ptr& inputQueue, QApplication& app);
    ~Drawer();

    void run(QApplication& app);

private:
    drawings_queue_ptr inputQueue_;
    bool threadRunning_;
    std::string logger_;
};

#endif // DRAWER_HPP
