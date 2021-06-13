#ifndef _PASSENGER_HPP_
#define _PASSENGER_HPP_

#include "Destination.hpp"
#include "Messages.hpp"
#include <string>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>

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
    bool alive_ = true;

public:
    Passenger(int id, Destination dest = Destination::London, int gnumber = 0)
        : ID_((id >= 0) ? 0 : id), Destination_(dest), gateNumber_(gnumber), alive_(true)
    {
        MessageQueueName_ = "passenger:" + std::to_string(ID_) + "MessageQueue";
    };
    ~Passenger()
    {

    };

    // Copy Contructor
    Passenger(const Passenger &oldPassenger) 
        : ID_(oldPassenger.ID_), Destination_(oldPassenger.Destination_), atGate_(oldPassenger.atGate_),
          gateNumber_(oldPassenger.gateNumber_), MessageQueueName_(oldPassenger.MessageQueueName_)
    {};
    // Move Contructor
    explicit Passenger(const Passenger &&oldPassenger) noexcept
    {
        ID_ = std::move(oldPassenger.ID_);
        Destination_ = std::move(oldPassenger.Destination_);
        atGate_ = std::move(oldPassenger.atGate_);
        gateNumber_ = std::move(oldPassenger.gateNumber_);
        MessageQueueName_ = std::move(oldPassenger.MessageQueueName_);
    };

    Passenger &operator=(const Passenger &passenger) // copy ass op
    {
        Passenger copyPassenger(passenger);
        std::swap(*this, copyPassenger);
        return *this;
    };

    Passenger &operator=(Passenger &&oldPassenger) noexcept // move ass op
    {
        if (this != &oldPassenger)
        {
            ID_ = std::move(oldPassenger.ID_);
            Destination_ = std::move(oldPassenger.Destination_);
            atGate_ = std::move(oldPassenger.atGate_);
            gateNumber_ = std::move(oldPassenger.gateNumber_);
            MessageQueueName_ = std::move(oldPassenger.MessageQueueName_);
        }
        else
        {
            operator=(oldPassenger);
        }
        return *this;
    };

    void sendDestination()
    {
        // message_queue Passenger_(open_or_create, "AirportMessagesQueue", 100, sizeof(Messages::PassengerToAirportController));
        // Messages::PassengerToAirportController message;
        // message.Destination_ = Destination_;
        // message.PassengerMsgQ = MessageQueueName_;
        // Passenger_.send(&message, sizeof(message), 0);

        Messages::PassengerToAirportController message;
        message.Destination_ = Destination_;
        message.PassengerID_ = ID_;
        SendMessage<Messages::PassengerToAirportController>("AirportMessagesQueue", message);
        // std::cout << "Sent destination to Airpotcontroller:" << Destination_ << std::endl;
    };
    void recieveGateNumber()
    {
        // message_queue Passenger_(open_or_create, MessageQueueName_.c_str(), 100, sizeof(Messages::AirportControllerToPassenger));
        // Messages::AirportControllerToPassenger * msg = new Messages::AirportControllerToPassenger;
        // message_queue::size_type recvd_size;
        // unsigned int priority = 0;
        // Passenger_.receive(&msg, sizeof(Messages::AirportControllerToPassenger), recvd_size, priority);
        auto msg = receiveMessage<Messages::AirportControllerToPassenger>(MessageQueueName_);
        gateNumber_ = msg.GateNumber;
    };
    void moveToGate()
    {
        Messages::passengerInfo msg;
        msg.passengerAddr = this;

        std::string mqName = "gate" + std::to_string(gateNumber_) + "passengers";

        // message_queue mq(open_or_create, mqName.c_str(), 100, sizeof(Messages::passengerInfo));

        // unsigned int priority = 0;

        // mq.send(&msg, sizeof(msg), priority);

        SendMessage<Messages::passengerInfo>(mqName, msg);
        atGate_ = true;
    };

    void setAlive(bool state)
    {
        alive_ = state;
    };
    bool getAlive()
    {
        return alive_;
    };
};
#endif