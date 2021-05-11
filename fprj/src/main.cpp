#include "Passenger.hpp"
#include <thread>
#include <string>



void PassengerCreatorTheat(int seed)
{
    using namespace std::literals;
    while(1)
    {
        int PassengerID = 1;
        Passenger PassengerObjekt(PassengerID, Destination::London);
        std::string threadName =  "passenger:" + std::to_string(PassengerID) + "theat"; 
        std::thread threadName(PassengerTheat, &PassengerObjekt );
        //sleep(1);
        std::this_thread::sleep_for(1000ms);
        PassengerID++;
    };
};

void AirplaneCreatorTheat(int seed)
{

};

void AirplaneTheat(int seed)
{

};

void PassengerTheat(Passenger PassengerObjekt)
{   
   
        PassengerObjekt.sendDestination()
        PassengerObjekt.recieveGateNumber()
        white for gatenumber
        Passenger.moveToGate()
    

};

void ControlTowerTheat(int seed)
{

};

void GateTheat(int seed)
{

};

void AirplaneContrellerTheat(int seed)
{

};


int main() 
{   
    std::thread PassengerCreatorTheat(PassengerCreatorTheat,);
    std::thread AirplaneCreatorTheat(AirplaneCreatorTheat,);
    std::thread ControlTowerTheat(ControlTowerTheat,);
    std::thread GateTheat(GateTheat);
    std::thread AirplaneContrellerTheat(AirplaneContrellerTheat,);













}