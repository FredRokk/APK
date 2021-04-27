#ifndef _MESSAGES_HPP_
#define _MESSAGES_HPP_
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
    };

    struct AirportControllerToPassenger : public Message
    {
        int GateNumber;
    };
}


//message_queue fromPlaneToTower_(open_or_create, "tower_msgs", 100, sizeof(Messages::PlaneToTower));

#endif /*_MESSAGES_HPP_*/