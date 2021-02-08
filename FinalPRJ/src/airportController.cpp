/*
 * Filename: /home/stud/APK/FinalPRJ/src/airportController.cpp
 * Path: /home/stud/APK/FinalPRJ/src
 * Created Date: Monday, November 23rd 2020, 3:48:15 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 *
 * Copyright (c) 2020 APK - Group 4
 */

#include "airplane.hpp"
//#include "bagage.hpp"
#include "controlTower.hpp"
#include "gate.hpp"
//#include "messages.hpp"
#include "passengerGenerator.hpp"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#define LEFT_AIRPORT "src/PassengersThatHasLeftTheAirport.log"
#define ENTERED_AIRPORT "src/PassengersThatHasEnteredTheAirport.log"

using namespace boost::interprocess;
typedef std::pmr::vector<passenger> passengerList;
boost::mutex                        io_mutex;

template<class T>
void writeToLog(std::vector<T> &container, const std::string &filename) // use for std::vector
{
  std::ofstream fileStream(filename.c_str(), std::ios::app);
  std::copy(container.begin(), container.end(), std::ostream_iterator<T>(fileStream, "\n"));
}

template<class T>
void writeToLog(std::pmr::vector<T> &container, const std::string &filename) // use for std::pmr::vector
{
  std::ofstream fileStream(filename.c_str(), std::ios::app);
  std::copy(container.begin(), container.end(), std::ostream_iterator<T>(fileStream, "\n"));
}

void controlTowerThread(controlTower *ct)
{
  while (1)
  {
    ct->ts_handleAirplane();
  }
}

template <int CargoCapacity, int PassengerCapacity, int Size, int Reach>
void airplaneThread(
    airplane<CargoCapacity, PassengerCapacity, Size, Reach> *plane,
    std::vector<gate> *gates_, passengerList *passengers,
    passengerGenerator *passGen)
{
  boost::mutex mtx;
  bool         leaving;
  passengerList passengersFromLog;
  do
  {
    leaving = plane->isLeaving();
    if (leaving)
    {
      // Fill passengers
      bool  gateFound_f = false; // flag thats high if a free gate was found
      gate *currentGate = nullptr;
      do
      {
        boost::unique_lock<boost::mutex> scoped_lock(mtx);
        for (auto itr = gates_->begin(); itr != gates_->end(); itr++)
        {
          if (!itr->isOccupied() && !gateFound_f)
          {
            currentGate = &*itr; // How beautiful!
            gateFound_f = true;
          }
        }
      } while (!gateFound_f);
      gateFound_f = false;
      currentGate->fillAirplane(*plane, *passengers);
#ifdef DEBUG
      std::vector<passenger> tempVec = plane->getPassengers();
      std::for_each(tempVec.begin(), tempVec.end(), [](passenger &pass) {
        io_mutex.lock();
        std::cout << pass << std::endl;
        io_mutex.unlock();
      });
#endif

      // Taxi
      plane->ts_taxi();

      // Lift Off
      plane->ts_takeOff();

      // Call destructor
      io_mutex.lock();
      std::cout << plane->getTailNr() << " has left the airport!" << std::endl;
      io_mutex.unlock();

      // Write passengers into database before destruction
      io_mutex.lock();
      // readPassengerLog(passengersFromLog, PASSENGER_LOG);
      // passengersFromLog.insert(passengersFromLog.end(),plane->getPassengers().begin(), plane->getPassengers().end());
      writeToLog<passenger>(plane->getPassengers(), LEFT_AIRPORT);
      io_mutex.unlock();

      plane->~airplane();

      // Join in main
    }
    else
    {
      // Fill airplane with passengers before landing
      passengerList &tempPassVec = plane->getPassengers();
      for (size_t i = 0; i < PassengerCapacity; i++)
      {
        tempPassVec.push_back(passGen->generatePassenger(1, 15));
      }
      // Land
      plane->ts_touchDown();
      // Taxi
      plane->ts_taxi();
      // Empty Passengers
      bool  gateFound_f = false; // flag thats high if a free gate was found
      gate *currentGate = nullptr;
      do
      {
        boost::unique_lock<boost::mutex> scoped_lock(mtx);
        for (auto itr = gates_->begin(); itr != gates_->end(); itr++)
        {
          if (!itr->isOccupied())
          {
            currentGate = &*itr; // How beautiful!
          }
        }
      } while (!gateFound_f);
      gateFound_f = false;

      // Log entering passengers
      io_mutex.lock();
      writeToLog<passenger>(plane->getPassengers(), ENTERED_AIRPORT);
      io_mutex.unlock();

      currentGate->emptyAirplane(*plane, *passengers);

      io_mutex.lock();
      std::cout << plane->getTailNr() << " has entered the airport!"
                << std::endl;
      io_mutex.unlock();

      // set to leving and leave the airport
      plane->changeLeaving();
    }
  } while (!leaving);
}

int main()
{
  std::vector<runway *> runways;
  runway                strip1(1, 1500);
  runway                strip2(2, 700);
  runway                strip3(3, 1500);

  runway *strip1Ptr = &strip1;
  runway *strip2Ptr = &strip2;
  runway *strip3Ptr = &strip3;

  runways.push_back(strip1Ptr);
  runways.push_back(strip2Ptr);
  runways.push_back(strip3Ptr);

  std::vector<gate> gates;
  gate              gate1(1);
  gate              gate2(2);
  gate              gate3(3);

  gates.push_back(gate1);
  gates.push_back(gate2);
  gates.push_back(gate3);

  std::pmr::vector<passenger> passVec;
  passengerGenerator          passGen;
  for (size_t i = 0; i < 500; i++)
  {
    passVec.push_back(passGen.generatePassenger(1, 15));
  }

  airplane<100, 200, 1500, 13000> test("at gate", "TEST1");
  airplane<100, 200, 1500, 13000> test2("in air", "TEST2", false);

  controlTower tower1(runways);

  try
  {
    message_queue::remove("TEST1");
    message_queue::remove("TEST2");
    message_queue::remove("tower_msgs");
  }
  catch (const interprocess_exception &ex)
  {
    std::cout << ex.what() << std::endl;
    return (1);
  }

  boost::thread towerThread(&controlTowerThread, &tower1);
  boost::thread testThread1(&airplaneThread<100, 200, 1500, 13000>, &test,
                            &gates, &passVec, &passGen);
  boost::thread testThread2(&airplaneThread<100, 200, 1500, 13000>, &test2,
                            &gates, &passVec, &passGen);

  towerThread.join();
  testThread1.join();
  testThread2.join();

  return 0;
}