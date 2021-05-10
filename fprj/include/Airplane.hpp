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
    int RunwayID_;
    enum sizes
    {
        small,
        medium,
        big
    };
    


public:
    Airplane(int FlightID = 1, int Capacity = 200, sizes size = big, Destination dis = Destination::Oslo) 
        : FlightID_(FlightID), Capacity_(Capacity), Size_(size), Destination_(dis), RunwayID_(0)
    {
        // FlightID_ = FlightID;
        // Capacity_ = Capacity;
        // Size_ = size;
        // Destination_ = dis;
    };
    ~Airplane()
    {

    };
    void ReceiveGeneralInfo()
    {
        std::string FlightID = std::to_string(FlightID_);
        message_queue Plane_(open_or_create, FlightID.c_str(), 100,
                           sizeof(Messages::AirportControllerToAirplane));

        Messages::AirportControllerToAirplane *msg = new Messages::AirportControllerToAirplane;

        message_queue::size_type recvd_size;
        unsigned int             priority = 0;

        Plane_.try_receive(msg, sizeof(Messages::AirportControllerToAirplane), recvd_size,
                         priority);
        GateNumber_ = msg->GateNumber;
        Destination_ = msg->Destination_;
    };
    void SendConfirmation()
    {
        message_queue PlaneConformation_(open_or_create, "PlaneConfirmationQueue", 100, sizeof(Messages::PlaneConfirmation));
        Messages::PlaneConfirmation msg;
        msg.GateID = GateNumber_;
        msg.FlightID = FlightID_;
        PlaneConformation_.send(&msg, sizeof(Messages::PlaneConfirmation), 0);
    };
    void AskForRunway()
    {

    };
    void ReceiveRunway()
    {

    };
    void SendIsFull()
    {
        /* Boost signals? */
    };
    bool GetIsFull() const
    {
        return IsFull_;
    };
    void BoardPassenger(Passenger Passenger){};
    void TakeOff()
    {
        
    };
    int getID() const
    {
        return FlightID_;
    };
    int getCapacity() const
    {
        return Capacity_;
    };
};
#endif