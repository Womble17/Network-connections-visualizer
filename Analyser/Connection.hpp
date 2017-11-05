#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>

class Connection
{
public:
    Connection(std::string IP);

    int counter_;
    float geoLatitude_;
    float geoLongtitude_;
    std::string IP_;

    void findGeolocation();
private:
    std::string logger_;

};

#endif // CONNECTION_HPP
