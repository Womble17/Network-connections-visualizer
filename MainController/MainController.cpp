#include "MainController.hpp"

#include <iostream>
#include <string>
#include <thread>

#include "Analyser/Analyser.hpp"
#include "Reader/Reader.hpp"
#include "Drawer/Drawer.hpp"

using namespace std;

MainController::MainController()
{
    logger_ = "\n[MAIN CONTROLLER] ";

    cout << logger_ << "Constructor";
}

void MainController::run()
{

    std::cout << logger_ << "Main runner";

    queue_ptr queue_1 = make_shared<queue_type>(100);
    queue_ptr queue_2 = make_shared<queue_type>(100);

    unique_ptr<Reader> reader(new Reader(queue_1));
    unique_ptr<Analyser> analyser(new Analyser(queue_1, queue_2));
}

