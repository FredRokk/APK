#ifndef _GATE_HPP_
#define _GATE_HPP_

#include "Airplane.hpp"
#include "Event_signals.hpp"
#include "Passenger.hpp"
#include "timer.hpp"
#include <algorithm>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <list>
#include <string>

class Gate
{
private:
  /* data */
  int                    GateNumber_;
  bool                   GateOccupied_;
  std::list<Passenger *> PassengerList_;
  Airplane *             plane_;

public:
  Gate(int gateNumber)
      : GateNumber_((gateNumber >= 0) ? gateNumber : 0), GateOccupied_(false)
  {
    message_queue::remove(
        ("gate" + std::to_string(GateNumber_) + "passengers").c_str());
    message_queue::remove(
        ("gate" + std::to_string(GateNumber_) + "plane").c_str());
  };
  ~Gate(){};
  void addPassenger(Passenger &passenger)
  {
    PassengerList_.push_back(&passenger);
  };
  int  GetGateNumber() const { return GateNumber_; };
  void SetGateNumber(int ID)
  {
    if (ID >= 0) GateNumber_ = ID;
  };
  bool GetOccupancy() { return GateOccupied_; };
  void setOccupancy(bool state) { GateOccupied_ = state; };
  void BoardAirplane(Airplane &airplane)
  {
    timer tester;
    std::swap(PassengerList_, airplane.getPassengerList());
    // for(auto *pass : PassengerList_)
    // {
    //     airplane.getPassengerList().push_back(pass);
    // }
  };

  void receiveAirplaneInfo()
  {

    std::string mq_name = "gate" + std::to_string(GateNumber_) + "plane";

    auto message = receiveMessage<Messages::planeInfo>(mq_name);
    std::cout << "Gate" << GateNumber_
              << " Has received a essages::planeInfo message!" << std::endl;
    plane_        = static_cast<Airplane *>(message.planeAddr);
    GateOccupied_ = true;
  };

  void receivePassenger()
  {
    std::string mq_name = "gate" + std::to_string(GateNumber_) + "passengers";

    auto msg = receiveMessage<Messages::passengerInfo>(mq_name);
    PassengerList_.push_back(static_cast<Passenger *>(msg.passengerAddr));
  }

  void SendInitialInfoToAirportcontroller()
  {
    Messages::GateToAirportController message;
    message.GateAddr = this;

    SendMessage<Messages::GateToAirportController>("Gate2AirportCtrl", message);
    AirportControlllerWaitSignal();
  }

  Airplane *getAirplanePtr() const { return plane_; };
};
#endif