#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <chrono>
#include <queue>
#include <string>
#include <vector>

class LinkData
{
public:
    LinkData();

    void addPacket();
    unsigned updateTimestamps();

    unsigned getAmountOfPackets();

private:
    std::string logger_;
    std::vector<std::chrono::system_clock::time_point> timestamps_;

};

#endif // CONNECTION_HPP
