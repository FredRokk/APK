#ifndef _AIRPLANE_HPP_
#define _AIRPLANE_HPP_

#include "Destination.hpp"
#include "Messages.hpp"
#include "Passenger.hpp"
#include "Sizes.hpp"
#include <algorithm>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <list>
#include <string>

using namespace boost::interprocess;

class Airplane
{
private:
  /* data */
  int                    FlightID_;
  int                    Capacity_;
  sizes                  Size_;
  bool                   IsFull_;
  Destination            Destination_;
  int                    GateNumber_;
  std::list<Passenger *> PassengerList_;
  int                    RunwayID_;
  bool                   inUse;

public:
  Airplane(int FlightID = 1, int Capacity = 200, sizes size = sizes::big,
           Destination dis = Destination::London)
      : FlightID_(FlightID),
        Capacity_(Capacity),
        Size_(size),
        Destination_(dis),
        RunwayID_(0),
        GateNumber_(255),
        inUse(false){};
  ~Airplane()
  {
    std::for_each(PassengerList_.begin(), PassengerList_.end(),
                  [](Passenger *pass) { pass->setAlive(false); });
  };

  void sendPlaneInfoToAirportController()
  {
    Messages::planeInfo message;
    message.planeAddr = this;

    SendMessage<Messages::planeInfo>("InitialPlaneToAirportMessage", message);
  }

  void ReceiveGeneralInfo()
  {
    auto msg = receiveMessage<Messages::AirportControllerToAirplane>(
        std::to_string(FlightID_));

    GateNumber_  = msg.GateNumber;
    Destination_ = msg.Destination_;
    inUse        = true;
  };
  void SendConfirmation()
  {
    Messages::PlaneConfirmation msg;
    msg.GateID   = GateNumber_;
    msg.FlightID = FlightID_;

    SendMessage<Messages::PlaneConfirmation>("PlaneConfirmationQueue", msg);
  };
  void AskForRunway()
  {
    Messages::PlaneToControltower message;
    message.FlightID             = FlightID_;
    message.RequestingPermission = true;

    SendMessage<Messages::PlaneToControltower>("PlaneToTower", message);
  };
  bool ReceiveRunway()
  {
    auto message = receiveMessage<Messages::ControlTowerToPlane>(
        std::to_string(FlightID_));
    if (message.permission)
    {
      RunwayID_ = message.rwNumber;
    }
    bool permission = message.permission;

    return permission;
  };
  void SetIsFull() { IsFull_ = true; };
  bool GetIsFull() const { return IsFull_; };
  void BoardPassenger(Passenger &Passenger)
  {
    PassengerList_.push_back(&Passenger);
  };
  void TakeOff()
  {
    Messages::PlaneToControltower message;
    message.FlightID             = FlightID_;
    message.HasLeft              = true;
    message.RunwayID             = RunwayID_;
    message.RequestingPermission = false;

    SendMessage<Messages::PlaneToControltower>("PlaneToTower", message);
  };
  void SendPlaneInfoToGate()
  {
    Messages::planeInfo msg;
    msg.planeAddr = this;

    std::string gateMsgQueueName =
        "gate" + std::to_string(GateNumber_) + "plane";

    SendMessage<Messages::planeInfo>(gateMsgQueueName, msg);
  };
  int                     getID() const { return FlightID_; };
  int                     getCapacity() const { return Capacity_; };
  std::list<Passenger *> &getPassengerList() { return PassengerList_; };
  bool                    getInUse() const { return inUse; };
};
#endif