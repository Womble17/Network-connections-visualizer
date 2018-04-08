#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <chrono>
#include <queue>
#include <string>

class IPLocation
{
public:
    IPLocation(std::string IP);

    float geoLatitude_;
    float geoLongtitude_;

    void addPacket();
    void updateTimestamps();

private:
    std::string logger_;
    std::queue<std::chrono::system_clock::time_point> timestamps_;

    void findGeolocation();
    std::string IP_;
};

#endif // CONNECTION_HPP
