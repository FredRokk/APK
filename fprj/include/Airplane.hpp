#ifndef _AIRPLANE_HPP_
#define _AIRPLANE_HPP_

#include "Destination.hpp"
#include "Messages.hpp"
#include "Passenger.hpp"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <list>
#include <string>

using namespace boost::interprocess;

class Airplane
{
private:
  /* data */
  int                  FlightID_;
  int                  Capacity_;
  int                  Size_;
  bool                 IsFull_;
  Destination          Destination_;
  int                  GateNumber_;
  std::list<Passenger> PassengerList_;
  int                  RunwayID_;
  enum sizes
  {
    small,
    medium,
    big
  };

public:
  Airplane(int FlightID = 1, int Capacity = 200, sizes size = big,
           Destination dis = Destination::Oslo)
      : FlightID_(FlightID),
        Capacity_(Capacity),
        Size_(size),
        Destination_(dis),
        RunwayID_(0){
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
    std::string   FlightID = std::to_string(FlightID_);
    message_queue Plane_(open_or_create, FlightID.c_str(), 100,
                         sizeof(Messages::AirportControllerToAirplane));

    Messages::AirportControllerToAirplane *msg =
        new Messages::AirportControllerToAirplane;

    message_queue::size_type recvd_size;
    unsigned int             priority = 0;

    Plane_.try_receive(msg, sizeof(Messages::AirportControllerToAirplane),
                       recvd_size, priority);
    GateNumber_  = msg->GateNumber;
    Destination_ = msg->Destination_;
  };
  void SendConfirmation()
  {
    message_queue PlaneConformation_(open_or_create, "PlaneToTower", 100,
                                     sizeof(Messages::PlaneConfirmation));
    Messages::PlaneConfirmation msg;
    msg.GateID   = GateNumber_;
    msg.FlightID = FlightID_;
    PlaneConformation_.send(&msg, sizeof(msg), 0);
  };
  void AskForRunway()
  {
    Messages::PlaneToControltower message;
    unsigned int                  priority = 0;
    message_queue                 mq(open_or_create, "PlaneToTower", 100,
                     sizeof(Messages::PlaneToControltower));
    message.FlightID             = FlightID_;
    message.RequestingPermission = true;
    mq.send(&message, sizeof(message), priority);
  };
  bool ReceiveRunway()
  {
    Messages::ControlTowerToPlane *message = new Messages::ControlTowerToPlane;
    message_queue::size_type      recvd_size;
    unsigned int                  priority = 0;
    std::string                   FlightID = std::to_string(FlightID_);
    message_queue mq(open_or_create, FlightID.c_str(), 100, sizeof(Messages::ControlTowerToPlane));
    mq.receive(&message, sizeof(Messages::ControlTowerToPlane), recvd_size, priority);
    if (message->permission)
    {
        RunwayID_ = message->rwNumber;
    }
    bool permission = message->permission;
    delete message;
    return permission;
  };
  void SendIsFull() { IsFull_ = true; };
  bool GetIsFull() const { return IsFull_; };
  void BoardPassenger(Passenger &Passenger)
  {
    PassengerList_.push_back(Passenger);
  };
  void TakeOff()
  {
    Messages::PlaneToControltower message;
    unsigned int priority = 0;
    message_queue mq(open_or_create, "PlaneToTower", 100, sizeof(Messages::PlaneToControltower));
    message.FlightID = FlightID_;
    message.HasLeft = true;
    message.RunwayID = RunwayID_;
    mq.send(&message, sizeof(message), priority);
  };
  int getID() const { return FlightID_; };
  int getCapacity() const { return Capacity_; };
  std::list<Passenger> &getPassengerList()
  {
    return PassengerList_;
  };

};
#endif