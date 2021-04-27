#ifndef _PASSENGER_HPP_
#define _PASSENGER_HPP_

#include "Destination.hpp"
#include <string>

class Passenger
{
private:
    /* data */
    int ID_;
    Destination Destination_;
    bool atGate_;
    int gateNumber_;

public:
    Passenger(int id, Destination dest = Destination::London, int gnumber = 0)
        : ID_((id >= 0) ? 0 : id), Destination_(dest), gateNumber_(gnumber)
    {

    };
    ~Passenger()
    {

    };
    void sendDestination()
    {

    };
    int recieveGateNumber()
    {

    };
    void moveToGate()
    {

    };
};
#endif