#ifndef _AIRPORT_CONTROLLER_HPP_
#define _AIRPORT_CONTROLLER_HPP_
#include "Airplane.hpp"
#include "Destination.hpp"
#include "Gate.hpp"
#include "Messages.hpp"
#include <list>
#include <string>
#include <algorithm>

/*  msg queue receive to try_receive   */
class AirportController
{
private:
  /* data */
  std::list<Destination>                            Destinations_;
  std::list<Messages::PassengerToAirportController> PassengerMsgs_;
  std::list<Gate>                                   GateList_;
  std::list<Airplane>                               AirplaneList_;
  Gate *chosenGate = nullptr;

  int CheckNumberOfTravelersToDestination(Destination dest)
  {
    return std::count(Destinations_.begin(), Destinations_.end(), dest);
  }

  void FindGateByNumber(int gateNumber)
  {
    for(auto &gateItem : GateList_)
    {
      if(gateItem.GetGateNumber() == gateNumber)
      {
        chosenGate = &gateItem;
        break;
      }
    }
  }

  void RemovePlaneByID(int planeID)
  {
    Airplane *planePtr = nullptr;
    for (auto &airplaneItem : AirplaneList_)
    {
      if (airplaneItem.getID() == planeID)
      {
        *planePtr = airplaneItem;
        break;
      }
    }
    if (planePtr != nullptr)
    {
      AirplaneList_.remove(*planePtr);
    }
  }

  int FindFreeGate()
  {
    for (auto &gateItem : GateList_)
    {
      if (gateItem.GetOccupancy() == false)
      {
        gateItem.setOccupancy(true);
        return gateItem.GetGateNumber();
        break;
      }
    }
  }

public:
  AirportController(/* args */){};
  ~AirportController()
  {};

  void SendAirplaneInfo(int capacity, Destination dest) 
  {
    for (auto Plane : AirplaneList_)
    {
      if (Plane.getCapacity() >= capacity )
      {
        std::string FlightID = std::to_string(Plane.getID());
        message_queue AirportController_( open_or_create, FlightID.c_str(), 100, sizeof(Messages::AirportControllerToAirplane));

        Messages::AirportControllerToAirplane msg;

        msg.Destination_ = dest;
        msg.GateNumber = FindFreeGate();

        AirportController_.send(
            &msg, sizeof(Messages::AirportControllerToAirplane), 0);
      }
    }
  };

  void ReceiveAirplaneConfirmation()
  {
    message_queue Planeconfirmation_(
        open_or_create, "PlaneConfirmationQueue", 100, sizeof(Messages::PlaneConfirmation));

    Messages::PlaneConfirmation *msg = new Messages::PlaneConfirmation;

    message_queue::size_type recvd_size;
    unsigned int             priority = 0;

    Planeconfirmation_.try_receive(msg,
                               sizeof(Messages::PlaneConfirmation),
                               recvd_size, priority);
    int gate = msg->GateID;
    FindGateByNumber(gate);
    if(chosenGate != nullptr)
    {
      chosenGate->setOccupancy(true);
    }
                               
    delete msg;
  };

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
    bool receivedMessage = false;
    message_queue AirportController_(
        open_or_create, "AirportMessagesQueue", 100,
        sizeof(Messages::PassengerToAirportController));

    Messages::PassengerToAirportController *message =
        new Messages::PassengerToAirportController;

    message_queue::size_type recvd_size;
    unsigned int             priority = 0;

    receivedMessage = AirportController_.try_receive(message,
                               sizeof(Messages::PassengerToAirportController),
                               recvd_size, priority);
    if (receivedMessage)
    {
      AddDestination(message->Destination_);
      PassengerMsgs_.push_back(*message);
    }
    delete message;
  };
  
  void AddGate(Gate gate_) { GateList_.push_back(gate_); };
  void AddDestination(Destination destination_)
  {
    Destinations_.push_back(destination_);
  };
  void AddAirplane(Airplane plane_) { AirplaneList_.push_back(plane_); };

  void ReceiveAirplaneTakenOff(Airplane plane_)
  {
    bool receivedMessage = false;
    size_t recvd_size;
    unsigned int priority = 0;
    auto message = new Messages::PlaneHasLeft;

    message_queue mq(open_or_create, "FromTowerToAirport", 100, sizeof(Messages::PlaneHasLeft));
    receivedMessage = mq.try_receive(&message, sizeof(Messages::PlaneHasLeft), recvd_size, priority);

    if (receivedMessage)
    {
      RemovePlaneByID(message->FlightID);
    }

    delete message;
  };
};
#endif