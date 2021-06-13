#define DEBUG
#include "Airplane.hpp"
#include "AirportController.hpp"
#include "ControlTower.hpp"
#include "Gate.hpp"
#include "Passenger.hpp"
#include "Sizes.hpp"
#include "timer.hpp"
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>

#define NUM_PLANES 5

volatile bool initialized = false;

std::condition_variable airplaneCanDisapear;
std::mutex              mtx;

void AirplaneThreadFunction(Airplane airplaneObject)
{

  bool permission = false;

  airplaneObject.sendPlaneInfoToAirportController();
  std::cout << "Airplane"
            << "send plane info" << std::endl;
  airplaneObject.ReceiveGeneralInfo();
  std::cout << "Airplane"
            << "Receive General Info" << std::endl;
  airplaneObject.SendConfirmation();
  std::cout << "Airplane"
            << "Send confirmation" << std::endl;
  airplaneObject.SendPlaneInfoToGate();
  std::cout << "Airplane"
            << "Send plane info to gate" << std::endl;
  while (!airplaneObject.GetIsFull())
  {
  }

  std::cout << "\033[1;31mAirplane" << airplaneObject.getID()
            << " is starting to take off\033[0m" << std::endl;

  do
  {
    airplaneObject.AskForRunway();
    permission = airplaneObject.ReceiveRunway();
  } while (!permission);

  airplaneObject.TakeOff();

  std::cout << "\033[1;31mAirplane nr" << airplaneObject.getID()
            << " Has taken off\033[0m" << std::endl;

  std::unique_lock<std::mutex> lck(mtx);

  airplaneCanDisapear.wait(lck);
}

void PassengerThreadFunction(Passenger &&PassengerObject)
{

  PassengerObject.sendDestination();

  PassengerObject.recieveGateNumber();

  PassengerObject.moveToGate();

  while (PassengerObject.getAlive())
  {
  };
}

void GateThreadFunction(Gate &&gateObject)
{

  std::cout << "Created gate nr." << gateObject.GetGateNumber() << std::endl;
  Airplane *   planePtr      = nullptr;
  unsigned int planeCapacity = 0;
  gateObject.SendInitialInfoToAirportcontroller();
  std::cout << "Gate" << gateObject.GetGateNumber()
            << "Has sent its initial message to Airportcontroller" << std::endl;
  while (true)
  {
    gateObject.receiveAirplaneInfo();
    std::cout << "\033[1;31mPlane has arrived at gate "
              << gateObject.GetGateNumber() << "\033[0m" << std::endl;

    planePtr      = gateObject.getAirplanePtr();
    planeCapacity = planePtr->getCapacity();
    std::cout << "Plane Capacity is: " << planeCapacity << std::endl;
    for (size_t i = 0; i < planeCapacity; i++)
    {
      gateObject.receivePassenger();
      std::cout << "\033[1;31mGate" << gateObject.GetGateNumber()
                << " got a passenger: " << i << "\033[0m" << std::endl;
    }

    gateObject.BoardAirplane(*planePtr);

    planePtr->SetIsFull();

    planePtr = nullptr;

    std::cout << "Gate" << gateObject.GetGateNumber()
              << " is done with filling airplane" << std::endl;
  }
}

void AirportControllerThreadFunction(
    AirportController &&AirportControllerObject)
{

  using namespace std::literals;
  std::cout << "Hello from Airportcontroller" << std::endl;
  for (int i = Destination::London; i < Destination::TelAviv + 1; i++)
  {
    AirportControllerObject.initialReceiveGates(static_cast<Destination>(i));
    std::cout << "!!!Got a Gate!!! " << i << std::endl;
  }
  for (int i = 0; i < NUM_PLANES; i++)
  {
    AirportControllerObject.receiveInitialPlaneInfo();
    std::cout << "Airport receiving initial plane info" << std::endl;
  }
  initialized = true;
  while (true)
  {
    AirportControllerObject.ReceiveDestination();

    AirportControllerObject.ReceiveAirplaneTakenOff();
    if (AirportControllerObject.checkIfAirplanesIsEmpty())
    {
      airplaneCanDisapear.notify_all();
      std::this_thread::sleep_for(10s);
      exit(EXIT_SUCCESS);
    }
    std::this_thread::sleep_for(500ms);
  }
}

void PassengerCreatorThreadFunction(unsigned int seed = 0xDEADBEEF)
{
  std::default_random_engine         generator(seed);
  std::uniform_int_distribution<int> randomDest((int)Destination::London,
                                                (int)Destination::TelAviv);
  using namespace std::literals;
  int PassengerID = 1;
  std::this_thread::sleep_for(1s);
  while (PassengerID <= 100)
  {
    Passenger pass(PassengerID,
                   static_cast<Destination>(randomDest(generator)));

    std::thread test(PassengerThreadFunction, pass);
    test.detach();

    std::this_thread::sleep_for(1s);
    PassengerID++;
  }
}

void AirplaneCreatorThreadFunction(unsigned int seed = 0xDEADBEEF)
{
  std::default_random_engine         generator(seed);
  std::uniform_int_distribution<int> randomCap(5, 6);
  std::uniform_int_distribution<int> randomSize((int)sizes::small,
                                                (int)sizes::big);
  std::uniform_int_distribution<int> randomDest((int)Destination::London,
                                                (int)Destination::TelAviv);
  using namespace std::literals;
  for (unsigned int i = 0; i < NUM_PLANES; i++)
  {
    Airplane plane(i + 1, randomCap(generator),
                   static_cast<sizes>(randomSize(generator)),
                   static_cast<Destination>(randomDest(generator)));
    std::cout << "Created an airplane" << plane.getID() << std::endl;
    std::thread(AirplaneThreadFunction, plane).detach();
  }
}

int main()
{
  message_queue::remove("InitialPlaneToAirportMessage");
  message_queue::remove("PlaneConfirmationQueue");
  message_queue::remove("PlaneToTower");
  message_queue::remove("AirportMessagesQueue");
  message_queue::remove("Gate2AirportCtrl");

  using namespace std::literals;
  std::cout << "\033[1;31mEntered Main\033[0m" << std::endl;
  std::this_thread::sleep_for(1s);

  AirportController Airport;
  ControlTower      Tower(3);
  Gate              Gate1(1);
  Gate              Gate2(2);
  Gate              Gate3(3);
  Gate              Gate4(4);
  Gate              Gate5(5);
  std::thread       AirplaneContrellerThread(AirportControllerThreadFunction,
                                       Airport);
  std::thread       GateThread1(GateThreadFunction, Gate1);
  std::thread       GateThread2(GateThreadFunction, Gate2);
  std::thread       GateThread3(GateThreadFunction, Gate3);
  std::thread       GateThread4(GateThreadFunction, Gate4);
  std::thread       GateThread5(GateThreadFunction, Gate5);
  std::thread       AirplaneCreatorThread(AirplaneCreatorThreadFunction, 42);
  while (!initialized)
  {
  }
  std::thread PassengerCreatorThread(PassengerCreatorThreadFunction,
                                     0xBEEFCAFE);
  std::thread ControlTowerThread(Tower);

  PassengerCreatorThread.join();
  AirplaneCreatorThread.join();
  ControlTowerThread.join();
  GateThread1.join();
  GateThread2.join();
  GateThread3.join();
  GateThread4.join();
  GateThread5.join();
  AirplaneContrellerThread.join();
}