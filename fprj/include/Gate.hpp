#ifndef _GATE_HPP_
#define _GATE_HPP_

#include <list>
#include "Airplane.hpp"
#include "Passenger.hpp"

class Gate
{
private:
    /* data */
    int GateNumber_;
    bool GateOccupied_; 
    std::list<Passenger> PassengerList_;
     
public:
    Gate(int gateNumber)
        :GateNumber_((gateNumber >= 0) ? 0 : gateNumber), GateOccupied_(false)
    {};
    ~Gate()
    {};
    void addPassenger(Passenger &passenger)
    {
        PassengerList_.push_back(passenger);
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

};
#endif