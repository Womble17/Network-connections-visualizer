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

void MainController::run(QApplication& app)
{

    std::cout << logger_ << "Main runner";

    string_queue_ptr queue_1 = make_shared<string_queue_type>(1000);
    pair_queue_ptr queue_2 = make_shared<pair_queue_type>(1000);

    unique_ptr<Reader> reader(new Reader(queue_1));
    unique_ptr<Analyser> analyser(new Analyser(queue_1, queue_2));
    unique_ptr<Drawer> drawer(new Drawer(queue_2, app));
}

