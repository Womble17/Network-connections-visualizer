#include "Drawer.hpp"

#include <iostream>

using namespace std;

Drawer::Drawer()
{
    logger_ = "\n[DRAWER ] ";

    cout << logger_ << "Constructor";
}

void Drawer::run(int a){
    cout << logger_ << a;
}

