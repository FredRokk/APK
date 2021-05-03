#ifndef _AIRPORT_CONTROLLER_HPP_
#define _AIRPORT_CONTROLLER_HPP_
#include "Airplane.hpp"
#include "Destination.hpp"
#include "Gate.hpp"
#include "Messages.hpp"
#include <list>
#include <string>

class AirportController
{
private:
  /* data */
  std::list<Destination>                            Destinations_;
  std::list<Messages::PassengerToAirportController> PassengerMsgs_;
  std::list<int>                                    DestinationCapacity_;
  std::list<Gate>                                   GateList_;
  std::list<Airplane>                               AirplaneList_;

public:
  AirportController(/* args */){};
  ~AirportController()
  {
    Destinations_.empty();
    DestinationCapacity_.empty();
    GateList_.empty();
    AirplaneList_.empty();
  };
  void SendAirplainInfo(){};
  void SendGateToPassenger(const Destination destination)
  {
    for (auto PassengerMsg : PassengerMsgs_)
    {
      if (PassengerMsg.Destination_ == destination)
      {
        message_queue AirportController_(
            open_or_create, PassengerMsg.PassengerMsgQ.c_str(), 100,
            sizeof(Messages::AirportControllerToPassenger));

        Messages::AirportControllerToPassenger message;

        message.GateNumber = 1; // to be changed

        AirportController_.send(
            &message, sizeof(Messages::AirportControllerToPassenger), 0);

        PassengerMsgs_.remove(PassengerMsg);
      }
    }
  };
  void ReceiveDestination()
  {
    message_queue AirportController_(
        open_or_create, "AirportMessagesQueue", 100,
        sizeof(Messages::PassengerToAirportController));

    Messages::PassengerToAirportController *message =
        new Messages::PassengerToAirportController;

    message_queue::size_type recvd_size;
    unsigned int             priority = 0;

    AirportController_.receive(message,
                               sizeof(Messages::PassengerToAirportController),
                               recvd_size, priority);
                               
    AddDestination(message->Destination_);
    PassengerMsgs_.push_back(*message);
    delete message;
  };
  void ReceiveAirplaneConfirmation(){};
  void AddGate(Gate gate_) { GateList_.push_back(gate_); };
  void AddDestination(Destination destination_)
  {
    Destinations_.push_back(destination_);
  };
  void AddAirplane(Airplane plane_) { AirplaneList_.push_back(plane_); };
  void ReceiveAirplaneTakenOff(Airplane plane_){};
};
#endif