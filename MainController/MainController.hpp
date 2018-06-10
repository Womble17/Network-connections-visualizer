#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <memory>
#include <string>

#include <QApplication>

#include <boost/lockfree/spsc_queue.hpp>

#include <Protocols/Aliases.hpp>


class MainController
{
public:
    MainController();

    void run(QApplication& app);


private:
    std::string logger_;

};

#endif // MAINCONTROLLER_HPP
