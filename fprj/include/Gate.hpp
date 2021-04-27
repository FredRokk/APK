#ifndef _GATE_HPP_
#define _GATE_HPP_

#include <list>
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
    {

    };
    ~Gate()
    {

    };
    void addPassenger(int ID)
    {
        PassengerList_.push_back(Passenger(ID));

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
    void BoardAirplane()
    {

    };

};
#endif