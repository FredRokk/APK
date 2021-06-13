#ifndef _GATE_HPP_
#define _GATE_HPP_

#include <list>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <string>
#include <iostream>
#include "Airplane.hpp"
#include "Passenger.hpp"
#include "Event_signals.hpp"

class Gate
{
private:
    /* data */
    int GateNumber_;
    bool GateOccupied_; 
    std::list<Passenger *> PassengerList_;

    Airplane *plane_;
     
public:
    Gate(int gateNumber)
        :GateNumber_((gateNumber >= 0) ? gateNumber : 0), GateOccupied_(false)
    {
        message_queue::remove(("gate" + std::to_string(GateNumber_) + "passengers").c_str());
        message_queue::remove(("gate" + std::to_string(GateNumber_) + "plane").c_str());
    };
    ~Gate()
    {};
    void addPassenger(Passenger &passenger)
    {
        PassengerList_.push_back(&passenger);
    };
    int GetGateNumber() const
    {
        return GateNumber_;
    };
    void SetGateNumber(int ID)
    {
        if (ID >= 0)
            GateNumber_ = ID;
    };
    bool GetOccupancy()
    {    
        return GateOccupied_;
    };
    void setOccupancy(bool state)
    {
        GateOccupied_ = state;
    };
    void BoardAirplane(Airplane &airplane)
    {
        std::swap(PassengerList_, airplane.getPassengerList());
    };

    void receiveAirplaneInfo()
    {
    //   Messages::planeInfo *message = new Messages::planeInfo;
    //   message_queue::size_type recvd_size;
    //   unsigned int             priority = 0;

      std::string mq_name = "gate" + std::to_string(GateNumber_) + "plane";

    //   message_queue mq(open_or_create, mq_name.c_str(), 100, sizeof(Messages::planeInfo));
    //   mq.receive(&message, sizeof(Messages::planeInfo), recvd_size, priority);
        auto message = receiveMessage<Messages::planeInfo>(mq_name);
        std::cout << "Gate" << GateNumber_ << " Has received a essages::planeInfo message!" << std::endl;
      plane_ = static_cast<Airplane *>(message.planeAddr);
      GateOccupied_ = true;

      //delete message;
    };

    void receivePassenger()
    {
        // Messages::passengerInfo *msg = new Messages::passengerInfo;
        // message_queue::size_type recvd_size;
        // unsigned int             priority = 0;

        std::string mq_name = "gate" + std::to_string(GateNumber_) + "passengers";

        // message_queue mq(open_or_create, mq_name.c_str(), 100, sizeof(Messages::passengerInfo));
        // mq.receive(&msg, sizeof(Messages::passengerInfo), recvd_size, priority);
        auto msg = receiveMessage<Messages::passengerInfo>(mq_name);
        PassengerList_.push_back(static_cast<Passenger *>(msg.passengerAddr));
        //delete msg;
    }

    void SendInitialInfoToAirportcontroller()
    {
        Messages::GateToAirportController message;
        message.GateAddr = this;

        SendMessage<Messages::GateToAirportController>("Gate2AirportCtrl", message);
        AirportControlllerWaitSignal();
        // message_queue mq(open_or_create, "Gate2AirportCtrl", 100,
        //                  sizeof(Messages::GateToAirportController));
        // std::cout << "Opened or created message queue"
        //           << "Gate2AirportCtrl" << std::endl;
        // mq.send(&message, sizeof(message), 0);
        // std::cout << " send message " << message.GateAddr << std::endl;
    }

    Airplane *getAirplanePtr() const
    {
        return plane_;
    };
};
#endif