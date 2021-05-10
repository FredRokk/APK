#ifndef _MESSAGES_HPP_
#define _MESSAGES_HPP_
#include <string>
#include "Destination.hpp"

class Message
{
public:
    virtual ~Message() = default;
};

namespace Messages 
{
    struct ControlTowerToPlane : public Message
    {
        bool permission;
        int  rwNumber;
    };

    struct PassengerToAirportController : public Message
    {
        Destination Destination_;
        std::string PassengerMsgQ;
    };

    struct AirportControllerToPassenger : public Message
    {
        int GateNumber;
    };

    struct PlaneToControltower : public Message
    {
        int FlightID;
        bool RequestingPermission;
        bool HasLeft;
        int RunwayID;
    };

    struct PlaneConfirmation : public Message
    {
        int GateID;
        int FlightID;
    };
    
    struct PlaneHasLeft : public Message
    {
        int FlightID;
    };

     struct AirportControllerToAirplane : public Message
    {
        Destination Destination_;
        int GateNumber;
    };

}


//message_queue fromPlaneToTower_(open_or_create, "tower_msgs", 100, sizeof(Messages::PlaneToTower));

#endif /*_MESSAGES_HPP_*/
        
    