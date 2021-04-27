#ifndef _AIRPLANE_HPP_
#define _AIRPLANE_HPP_

#include <string>
#include <list>
#include "Messages.hpp"
#include "Passenger.hpp"
#include "Destination.hpp"
#include <boost/interprocess/ipc/message_queue.hpp>

using namespace boost::interprocess;

class Airplane
{
private:
    /* data */
    int FlightID_;
    int Capacity_;
    int Size_;
    bool IsFull_;
    Destination Destination_;
    int GateNumber_;
    std::list<Passenger> PassengerList;
    enum sizes
    {
        small,
        medium,
        big
    };
    


public:
    Airplane(int FlightID = 1, int Capacity = 200, sizes size = big, Destination dis = Destination::Oslo) 
        : FlightID_(FlightID), Capacity_(Capacity), Size_(size), Destination_(dis)
    {
        // FlightID_ = FlightID;
        // Capacity_ = Capacity;
        // Size_ = size;
        // Destination_ = dis;
    };
    ~Airplane(){};
    void ReceiveGeneralInfo(){};
    void SendConfirmation(){};
    void AskForRunway(){};
    void ReceiveRunway(){};
    void SendIsFull(){};
    bool GetIsFull() const
    {
        return IsFull_;
    };
    void BoardPassenger(Passenger Passenger){};
    void TakeOff(){};
};
#endif