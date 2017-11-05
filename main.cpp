#include <iostream>
#include <memory>

#include <MainController/MainController.hpp>

using namespace std;

int main()
{
    string logger_ = "\n[MAIN FUNCTION] ";
    cout << logger_ << "Starting program";

    cout << logger_ << "Starting Main Controller\n";

    std::unique_ptr<MainController> mainController(new MainController());

    mainController->run();

    cout << "\n" << logger_ << "Finishing program\n\n" << std::endl;

    return 0;
}



