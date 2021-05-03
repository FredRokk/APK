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
    std::string MessageQueueName_;

public:
    Passenger(int id, Destination dest = Destination::London, int gnumber = 0)
        : ID_((id >= 0) ? 0 : id), Destination_(dest), gateNumber_(gnumber)
    {
        MessageQueueName_ = "passenger:" + ID_;
    };
    ~Passenger()
    {

    };
    void sendDestination()
    {
        message_queue Passenger_(open_or_create, "AirportMessagesQueue", 100, sizeof(Messages::PassengerToAirportController));
        Messages::PassengerToAirportController message;
        message.Destination_ = Destination_;
        message.PassengerMsgQ = MessageQueueName_;
        Passenger_.send(&message, sizeof(Messages::PassengerToAirportController), 0);
    };
    int recieveGateNumber()
    {
        message_queue Passenger_(open_or_create, MessageQueueName_.c_str(), 100, sizeof(Messages::AirportControllerToPassenger));
        Messages::AirportControllerToPassenger * msg = new Messages::AirportControllerToPassenger;
        message_queue::size_type recvd_size;
        unsigned int priority = 0;
        Passenger_.receive(&msg, sizeof(Messages::AirportControllerToPassenger), recvd_size, priority);
        gateNumber_ = msg->GateNumber;
    };
    void moveToGate()
    {

    };
};
#endif