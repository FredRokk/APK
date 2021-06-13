#ifndef _AIRPORT_CONTROLLER_HPP_
#define _AIRPORT_CONTROLLER_HPP_
#include "Airplane.hpp"
#include "Destination.hpp"
#include "Event_signals.hpp"
#include "Gate.hpp"
#include "Messages.hpp"
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
/*  msg queue receive to try_receive   */
class AirportController
{
private:
  struct GateAndDestination_
  {
    Gate *      gate;
    Destination destination;
  };
  std::list<GateAndDestination_> GND_;
  std::list<Airplane *>          AirplaneList_;
  Gate *                         chosenGate = nullptr;

  void FindGateByNumber(int gateNumber)
  {
    for (auto &gnd : GND_)
    {
      if (gnd.gate->GetGateNumber() == gateNumber)
      {
        chosenGate = gnd.gate;
        break;
      }
    }
  }

  Gate *findGateFromDestination(Destination dest)
  {
    for (auto &gnd : GND_)
    {
      if (gnd.destination == dest)
      {
        chosenGate = gnd.gate;
        return chosenGate;
      }
    }
  }

  void RemovePlaneByID(int planeID)
  {
    Airplane *planePtr = nullptr;
    for (auto &airplaneItem : AirplaneList_)
    {
      if (airplaneItem->getID() == planeID)
      {
        planePtr = airplaneItem;
        break;
      }
    }
    if (planePtr != nullptr)
    {
      AirplaneList_.remove(planePtr);
    }
  }

public:
  AirportController(/* args */)
  {
    AirportControlllerWaitSignal.connect(&give);
  };
  ~AirportController(){};

  bool checkIfAirplanesIsEmpty() { return AirplaneList_.empty(); };

  void receiveInitialPlaneInfo()
  {
    auto msg =
        receiveMessage<Messages::planeInfo>("InitialPlaneToAirportMessage");

    AirplaneList_.push_back(static_cast<Airplane *>(msg.planeAddr));
  }

  void SendAirplaneInfo(Destination dest)
  {
    for (auto Plane : AirplaneList_)
    {
      if (Plane->getInUse() == false)
      {
        std::string FlightID = std::to_string(Plane->getID());

        Messages::AirportControllerToAirplane msg;

        msg.Destination_ = dest;
        findGateFromDestination(dest);
        if (chosenGate != nullptr)
        {
          msg.GateNumber = chosenGate->GetGateNumber();
          chosenGate     = nullptr;
        }

        SendMessage<Messages::AirportControllerToAirplane>(FlightID, msg);
        break;
      }
    }
  };

  bool ReceiveAirplaneConfirmation()
  {
    bool msg_rcvd = false;
    auto msg      = receiveMessage<Messages::PlaneConfirmation>(
        "PlaneConfirmationQueue", msg_rcvd);

    if (msg_rcvd)
    {
      int gate = msg.GateID;
      FindGateByNumber(gate);

      if (chosenGate != nullptr)
      {
        chosenGate->setOccupancy(true);
        chosenGate = nullptr;
      }
    }

    return msg_rcvd;
  };

  void ReceiveDestination()
  {
    bool receivedMessage = false;

    Messages::PassengerToAirportController msg1;
    msg1 = receiveMessage<Messages::PassengerToAirportController>(
        "AirportMessagesQueue", receivedMessage);
    if (receivedMessage)
    {
      Messages::AirportControllerToPassenger msg;
      findGateFromDestination(msg1.Destination_);

      if (!(chosenGate == nullptr))
      {
        msg.GateNumber = chosenGate->GetGateNumber();

        if (!chosenGate->GetOccupancy())
        {
          SendAirplaneInfo(msg1.Destination_);
        }

        chosenGate = nullptr;
      }
      else
      {
        std::cout << "No gate with destination of " << msg1.Destination_
                  << std::endl;
      }

      SendMessage<Messages::AirportControllerToPassenger>(
          ("passenger:" + std::to_string(msg1.PassengerID_) + "MessageQueue"),
          msg);
    }
  };

  void AddAirplane(Airplane &plane_) { AirplaneList_.push_back(&plane_); };

  void ReceiveAirplaneTakenOff()
  {
    bool receivedMessage = false;

    auto message = receiveMessage<Messages::PlaneHasLeft>("FromTowerToAirport",
                                                          receivedMessage);

    if (receivedMessage)
    {
      RemovePlaneByID(message.FlightID);
    }
  };

  void initialReceiveGates(Destination dest)
  {
    Messages::GateToAirportController msg;
    msg.GateAddr = nullptr;
    while (getWait() == 0)
    {
    }
    take();
    msg = receiveMessage<Messages::GateToAirportController>("Gate2AirportCtrl");
    Gate *gate = static_cast<Gate *>(msg.GateAddr);
    GND_.push_back(GateAndDestination_{.gate = gate, .destination = dest});
  };
};
#endif