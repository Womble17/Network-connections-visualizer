#include <iostream>
#include <string>

#include <MainController/MainController.hpp>

using namespace std;

int main()
{
    string logger_ = "\n[MAIN FUNCTION] ";
    cout << logger_ << "Starting program";

    cout << logger_ << "Starting Main Controller";
    MainController mainController;

    cout << logger_ << "Finishing program" << std::endl;

    return 0;
}



