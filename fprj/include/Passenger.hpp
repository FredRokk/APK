#ifndef _PASSENGER_HPP_
#define _PASSENGER_HPP_

#include "Destination.hpp"
#include "Messages.hpp"
#include <string>
#include <boost/interprocess/ipc/message_queue.hpp>

using namespace boost::interprocess;

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
        message_queue Passenger_(open_or_create, "AirportMessagesQueue", 100, sizeof(Messages::PassengerToAirportController));
        Messages::PassengerToAirportController *message = new Messages::PassengerToAirportController;
        message->Destination_ = Destination_;
        Passenger_.send(message, sizeof(Messages::PassengerToAirportController), 0);
    };
    int recieveGateNumber()
    {
        message_queue Passenger_(open_or_create, "PassengerMessagesQueue", 100, sizeof(Messages::AirportControllerToPassenger));

        //Passenger_.receive(Void * buffer, std::size_t buffer_size, std::size_t & recvd_size, 
        //    unsigned int & priority);

    };
    void moveToGate()
    {

    };
};
#endif