#include "Passenger.hpp"
#include "Airplane.hpp"
#include "Sizes.hpp"
#include <thread>
#include <string>
#include <random>



void PassengerCreatorThreadFunction(unsigned int seed = 0xDEADBEEF)
{
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<Destination> randomDest(Destination::London, Destination::TelAviv);
    using namespace std::literals;
    int PassengerID = 1;
    while(1)
    {
        Passenger PassengerObjekt(PassengerID, randomDest(generator));
        std::thread (PassengerThreadFunction, &PassengerObjekt).detach();
        
        std::this_thread::sleep_for(100ms);
        PassengerID++;
    }
}

void AirplaneCreatorThreadFunction(unsigned int seed = 0xDEADBEEF)
{
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> randomCap(20,300);
    std::uniform_int_distribution<sizes> randomSize(sizes::small, sizes::big);
    std::uniform_int_distribution<Destination> randomDest(Destination::London, Destination::TelAviv);
    using namespace std::literals;
    for (unsigned int i = 0; i < 20; i++)
    {
        Airplane plane(++i, randomCap(generator), randomSize(generator), randomDest(generator));
        std::thread (AirplaneThreadFunction, &plane).detach();
        std::this_thread::sleep_for(10ms);
    }
}

void AirplaneThreadFunction(Airplane airplaneObject)
{
    /*
    send initial information to airportController
    receive general information

    fly passengers away
    */
    airplaneObject.sendPlaneInfoToAirportController();
    airplaneObject.ReceiveGeneralInfo();
}

void PassengerThreadFunction(Passenger PassengerObjekt)
{   
   
    // PassengerObjekt.sendDestination();
    // PassengerObjekt.recieveGateNumber();
    // white for gatenumber
    // Passenger.moveToGate();
    

}

void ControlTowerThreadFunction(int seed)
{

}

void GateThreadFunction(int seed)
{

}

void AirportControllerThreadFunction(int seed)
{

}


int main() 
{   
    std::thread PassengerCreatorThread(PassengerCreatorThreadFunction, 0xBEEFCAFE);
    std::thread AirplaneCreatorThread(AirplaneCreatorThreadFunction,42);
    std::thread ControlTowerThread(ControlTowerThreadFunction);
    std::thread GateThread(GateThreadFunction);
    std::thread AirplaneContrellerThread(AirportControllerThreadFunction);













}