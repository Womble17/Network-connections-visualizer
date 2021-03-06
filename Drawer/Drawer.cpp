#include "Drawer.hpp"

#include <iostream>
#include <memory>

using namespace std;

Drawer::Drawer(drawings_queue_ptr& inputQueue, QApplication& app) :
    inputQueue_(inputQueue)
{
    logger_ = "\n[DRAWER]\n ";
    cout << logger_ << "Constructor ";

    run(app);
}

Drawer::~Drawer()
{
}

void Drawer::run(QApplication& app)
{
    Map* window = new Map(inputQueue_);

    window->resize(1400, 653);
    window->setWindowTitle("Network Connections Visualizer");

    QPalette palette;
    palette.setBrush(window->backgroundRole(), QBrush(QImage("world_truncated.jpg")));

    window->setPalette(palette);

    window->show();

    app.exec();

    delete window;
}

