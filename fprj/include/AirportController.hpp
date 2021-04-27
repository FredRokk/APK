#ifndef _AIRPORT_CONTROLLER_HPP_
#define _AIRPORT_CONTROLLER_HPP_
#include <list>
#include <string>
#include "Destination.hpp"
#include "Gate.hpp"
#include "Airplane.hpp"

class AirportController
{
private:
    /* data */
    std::list<Destination> Destinations_;
    std::list<int> DestinationCapacity_;
    std::list<Gate> GateList_;
    std::list<Airplane> AirplaneList_;
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
    void SendGateToPassenger()
    {
        message_queue AirportController_(open_or_create, "PassengerMessagesQueue", 100, sizeof(Messages::AirportControllerToPassenger));
        Messages::AirportControllerToPassenger *message = new Messages::AirportControllerToPassenger;
        message->GateNumber = 1; // to be changed

    };
    void ReceiveDestination()
    {
        message_queue AirportController_(open_or_create, "AirportMessagesQueue", 100, sizeof(Messages::PassengerToAirportController));
        AirportController_.receive();


    };
    void ReceiveAirplaneConfirmation(){};
    void AddGate(Gate gate_)
    {
        GateList_.push_back(gate_);
    };
    void AddDestination(Destination destination_)
    {
        Destinations_.push_back(destination_);
    };
    void AddAirplane(Airplane plane_)
    {
        AirplaneList_.push_back(plane_);
    };
    void ReceiveAirplaneTakenOff(Airplane plane_){};
};
#endif