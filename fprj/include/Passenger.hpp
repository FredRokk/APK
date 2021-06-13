#ifndef _PASSENGER_HPP_
#define _PASSENGER_HPP_

#include "Destination.hpp"
#include "Messages.hpp"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <string>

using namespace boost::interprocess;

class Passenger
{
private:
  /* data */
  int         ID_;
  Destination Destination_;
  bool        atGate_;
  int         gateNumber_;
  std::string MessageQueueName_;
  bool        alive_ = true;

public:
  Passenger(int id, Destination dest = Destination::London, int gnumber = 0)
      : ID_((id >= 0) ? 0 : id),
        Destination_(dest),
        gateNumber_(gnumber),
        alive_(true)
  {
    MessageQueueName_ = "passenger:" + std::to_string(ID_) + "MessageQueue";
  };
  ~Passenger(){

  };

  // Copy Contructor
  Passenger(const Passenger &oldPassenger)
      : ID_(oldPassenger.ID_),
        Destination_(oldPassenger.Destination_),
        atGate_(oldPassenger.atGate_),
        gateNumber_(oldPassenger.gateNumber_),
        MessageQueueName_(oldPassenger.MessageQueueName_){};
  // Move Contructor
  explicit Passenger(const Passenger &&oldPassenger) noexcept
  {
    ID_               = std::move(oldPassenger.ID_);
    Destination_      = std::move(oldPassenger.Destination_);
    atGate_           = std::move(oldPassenger.atGate_);
    gateNumber_       = std::move(oldPassenger.gateNumber_);
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
      ID_               = std::move(oldPassenger.ID_);
      Destination_      = std::move(oldPassenger.Destination_);
      atGate_           = std::move(oldPassenger.atGate_);
      gateNumber_       = std::move(oldPassenger.gateNumber_);
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
    Messages::PassengerToAirportController message;
    message.Destination_ = Destination_;
    message.PassengerID_ = ID_;
    SendMessage<Messages::PassengerToAirportController>("AirportMessagesQueue",
                                                        message);
  };
  void recieveGateNumber()
  {
    auto msg = receiveMessage<Messages::AirportControllerToPassenger>(
        MessageQueueName_);
    gateNumber_ = msg.GateNumber;
  };
  void moveToGate()
  {
    Messages::passengerInfo msg;
    msg.passengerAddr = this;

    std::string mqName = "gate" + std::to_string(gateNumber_) + "passengers";

    SendMessage<Messages::passengerInfo>(mqName, msg);
    atGate_ = true;
  };

  void setAlive(bool state) { alive_ = state; };
  bool getAlive() { return alive_; };
};
#endif