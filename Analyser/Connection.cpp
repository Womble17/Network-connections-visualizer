#include "Connection.hpp"

#include <iostream>

Connection::Connection()
{
    logger_ = "\n[CONNECTION] ";
    std::cout << logger_ << "Constructor ";
}

