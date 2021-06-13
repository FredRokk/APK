#ifndef _AIRPLANE_HPP_
#define _AIRPLANE_HPP_

#include "Destination.hpp"
#include "Messages.hpp"
#include "Passenger.hpp"
#include "Sizes.hpp"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <list>
#include <string>
#include <algorithm>

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
  bool inUse;

public:
  Airplane(int FlightID = 1, int Capacity = 200, sizes size = sizes::big,
           Destination dis = Destination::London)
      : FlightID_(FlightID),
        Capacity_(Capacity),
        Size_(size),
        Destination_(dis),
        RunwayID_(0),
        GateNumber_(255),
        inUse(false){
            // FlightID_ = FlightID;
            // Capacity_ = Capacity;
            // Size_ = size;
            // Destination_ = dis;
        };
  ~Airplane()
  {
    std::for_each(PassengerList_.begin(), PassengerList_.end(), [](Passenger *pass){
      pass->setAlive(false);
    });
  };

  void sendPlaneInfoToAirportController()
  {
    Messages::planeInfo message;
    message.planeAddr = this;

    // message_queue mq(open_or_create, "InitialPlaneToAirportMessage", 100,
    //                 sizeof(Messages::planeInfo));
    // mq.send(&message, sizeof(message), 0);

    SendMessage<Messages::planeInfo>("InitialPlaneToAirportMessage", message);
  }

  void ReceiveGeneralInfo()
  {
    // std::string   FlightID = std::to_string(FlightID_);
    // message_queue Plane_(open_or_create, FlightID.c_str(), 100,
    //                      sizeof(Messages::AirportControllerToAirplane));

    // Messages::AirportControllerToAirplane *msg =
    //     new Messages::AirportControllerToAirplane;

    // message_queue::size_type recvd_size;
    // unsigned int             priority = 0;

    // msg_rcvd =
    //     Plane_.try_receive(msg, sizeof(Messages::AirportControllerToAirplane),
    //                        recvd_size, priority);

    auto msg = receiveMessage<Messages::AirportControllerToAirplane>(std::to_string(FlightID_));

    GateNumber_  = msg.GateNumber;
    Destination_ = msg.Destination_;
    inUse = true;
  };
  void SendConfirmation()
{
    Messages::PlaneConfirmation msg;
    msg.GateID   = GateNumber_;
    msg.FlightID = FlightID_;

    // message_queue PlaneConformation_(open_or_create, "PlaneConfirmationQueue",
    //                                  100, sizeof(Messages::PlaneConfirmation));
    
    // PlaneConformation_.send(&msg, sizeof(msg), 0);

    SendMessage<Messages::PlaneConfirmation>("PlaneConfirmationQueue", msg);
  };
  void AskForRunway()
  {
    Messages::PlaneToControltower message;
    message.FlightID             = FlightID_;
    message.RequestingPermission = true;
    // unsigned int                  priority = 0;
    // message_queue                 mq(open_or_create, "PlaneToTower", 100,
    //                  sizeof(Messages::PlaneToControltower));
    
    // mq.send(&message, sizeof(message), priority);
    SendMessage<Messages::PlaneToControltower>("PlaneToTower", message);
  };
  bool ReceiveRunway()
  {
    // Messages::ControlTowerToPlane *message = new Messages::ControlTowerToPlane;
    // message_queue::size_type       recvd_size;
    // unsigned int                   priority = 0;
    // std::string                    FlightID = std::to_string(FlightID_);
    // message_queue                  mq(open_or_create, FlightID.c_str(), 100,
    //                  sizeof(Messages::ControlTowerToPlane));
    // mq.receive(&message, sizeof(Messages::ControlTowerToPlane), recvd_size,
    //            priority);
    auto message = receiveMessage<Messages::ControlTowerToPlane>(std::to_string(FlightID_));
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
    message.FlightID = FlightID_;
    message.HasLeft  = true;
    message.RunwayID = RunwayID_;
    message.RequestingPermission = false;

    // unsigned int                  priority = 0;
    // message_queue                 mq(open_or_create, "PlaneToTower", 100,
    //                  sizeof(Messages::PlaneToControltower));
    
    // mq.send(&message, sizeof(message), priority);

    SendMessage<Messages::PlaneToControltower>("PlaneToTower", message);
  };
  void SendPlaneInfoToGate()
  {
    Messages::planeInfo msg;
    msg.planeAddr = this;

    // unsigned int  priority         = 0;
    std::string   gateMsgQueueName = "gate" + std::to_string(GateNumber_) + "plane";
    // message_queue mq(open_or_create, gateMsgQueueName.c_str(), 100,
    //                  sizeof(Messages::planeInfo));

    // mq.send(&msg, sizeof(msg), priority);

    SendMessage<Messages::planeInfo>(gateMsgQueueName, msg);
  };
  int                     getID() const { return FlightID_; };
  int                     getCapacity() const { return Capacity_; };
  std::list<Passenger *> &getPassengerList() { return PassengerList_; };
  bool getInUse() const {return inUse;};
};
#endif