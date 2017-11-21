#include <iostream>
#include <memory>

#include <QApplication>

#include <MainController/MainController.hpp>

using namespace std;

int main(int argc, char** argv)
{
    string logger_ = "\n[MAIN FUNCTION] ";
    cout << logger_ << "Starting program";

    cout << logger_ << "Starting Main Controller\n";

    std::unique_ptr<MainController> mainController(new MainController());

    QApplication app(argc, argv);

    mainController->run(app);

    cout << "\n" << logger_ << "Finishing program\n\n" << std::endl;

    return 0;
}



