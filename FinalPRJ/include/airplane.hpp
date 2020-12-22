/*
 * Filename: /home/stud/APK/FinalPRJ/include/airplane.hpp
 * Path: /home/stud/APK/FinalPRJ/include
 * Created Date: Monday, November 23rd 2020, 3:42:58 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 *
 * Copyright (c) 2020 APK - Group 4
 */
#ifndef _AIRPLANE_
#define _AIRPLANE_

#include "messages.hpp"
#include "passenger.hpp"
#include "planeProperties.hpp"
#include "runway.hpp"
#include <boost/chrono.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
//#include <chrono>
#include <boost/thread.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace boost::interprocess;

template <int CargoCapacity, int PassengerCapacity, int Size, int Reach>
class airplane
    : public planeProperties<CargoCapacity, PassengerCapacity, Size, Reach>
{
private:
  const std::string      tailNumber_;
  std::vector<passenger> passengers_;
  std::string            destination_, position_;
  runway *               currentRunway_;
  bool                   leaving_;
  boost::mutex           mutex_;

public:
  airplane(std::string Position, std::string TailNumber, bool leaving = true)
      : position_(Position),
        tailNumber_(TailNumber),
        currentRunway_(nullptr),
        leaving_(leaving)
  {
    std::srand(std::time(NULL));
  };

  ~airplane(){
      // currentRunway_.~shared_ptr();
  };

  void ts_takeOff()
  {
    Messages::PlaneToTower sendMsg;
    sendMsg.TailNr            = tailNumber_;
    sendMsg.Position          = taking_off;
    sendMsg.RequestForLanding = false;
    sendMsg.RW                = *currentRunway_;
    position_                 = "taking off";

    try
    {
      message_queue fromPlaneToTower_(open_or_create, "tower_msgs", 100,
                                      sizeof(Messages::PlaneToTower));
      message_queue fromTowerToPlane_(open_or_create, tailNumber_.c_str(), 100,
                                      sizeof(Messages::TowerToPlane));
      fromPlaneToTower_.send(&sendMsg, sizeof(sendMsg), 0);
      {
        boost::unique_lock<boost::mutex> scoped_lock(mutex_);
        std::cout << tailNumber_ << " sent a message at takeoff" << std::endl;
      }
      boost::this_thread::sleep_for(boost::chrono::seconds(std::rand() % 10));
      {
        boost::unique_lock<boost::mutex> scoped_lock(mutex_);
        std::cout << tailNumber_ << " has taken off" << std::endl;
      }
      sendMsg.Position = in_air;
      fromPlaneToTower_.send(&sendMsg, sizeof(sendMsg), 0);
      boost::this_thread::sleep_for(boost::chrono::seconds(10));
      currentRunway_ = nullptr;
    }
    catch (const interprocess_exception &ex)
    {
      std::cout << ex.what() << " At function ts_takeOff()" << std::endl;
      exit(EXIT_FAILURE);
    }
  };

  void ts_touchDown()
  {
    Messages::TowerToPlane receiveMsg;
    Messages::PlaneToTower sendMsg;
    sendMsg.TailNr            = tailNumber_;
    sendMsg.Position          = in_air;
    sendMsg.RequestForLanding = true;
    position_                 = "in air";
    try
    {
      message_queue fromPlaneToTower_(open_or_create, "tower_msgs", 100,
                                      sizeof(Messages::PlaneToTower));
      message_queue fromTowerToPlane_(open_or_create, tailNumber_.c_str(), 100,
                                      sizeof(Messages::TowerToPlane));
      message_queue::size_type received_size;
      unsigned int             priority = 0;
      do
      {
        boost::this_thread::sleep_for(boost::chrono::seconds(std::rand() % 10));
        fromPlaneToTower_.send(&sendMsg, sizeof(sendMsg), 0);
        {
          boost::unique_lock<boost::mutex> scoped_lock(mutex_);
          std::cout << tailNumber_ << " sent a message at touchdown"
                    << std::endl;
        }
        fromTowerToPlane_.receive(&receiveMsg, sizeof(fromTowerToPlane_),
                                  received_size, priority);
        {
          boost::unique_lock<boost::mutex> scoped_lock(mutex_);
          std::cout << tailNumber_ << " received a message" << std::endl;
        }
        // if (received_size <= 0)
        // throw interprocess_exception("Received nothing!");
      } while (!receiveMsg.Permission); // Continue to ask for permission while
                                        // permission is no granted!
      currentRunway_ = &receiveMsg.RW;
      {
        boost::unique_lock<boost::mutex> scoped_lock(mutex_);
        std::cout << tailNumber_ << " has landed on runway"
                  << currentRunway_->getRunwayNumber() << std::endl;
      }
    }
    catch (const interprocess_exception &ex)
    {
      std::cout << ex.what() << " At function ts_touchDown()" << std::endl;
      exit(EXIT_FAILURE);
    }
  };

  void ts_taxi()
  {
    Messages::TowerToPlane receiveMsg;
    Messages::PlaneToTower sendMsg;
    if (leaving_)
    {
      sendMsg.Position = entering_runway;
      position_        = "entering runway";
    }
    else
    {
      sendMsg.Position = exiting_runway;
      sendMsg.RW       = *currentRunway_;
      position_        = "exiting runway";
    }
    sendMsg.RequestForLanding = false;
    sendMsg.TailNr            = tailNumber_;
    try
    {
      message_queue fromPlaneToTower_(open_or_create, "tower_msgs", 100,
                                      sizeof(Messages::PlaneToTower));
      message_queue fromTowerToPlane_(open_or_create, tailNumber_.c_str(), 100,
                                      sizeof(Messages::TowerToPlane));
      message_queue::size_type recieved_size;
      unsigned int             priority = 0;
      do
      {
        mutex_.lock();
        std::cout << tailNumber_ << " sending request to taxi to empty runway"
                  << std::endl;
        mutex_.unlock();
        fromPlaneToTower_.send(&sendMsg, sizeof(sendMsg), 0);
        fromTowerToPlane_.receive(&receiveMsg, sizeof(fromTowerToPlane_),
                                  recieved_size, priority);
        mutex_.lock();
        std::cout << tailNumber_ << " received a message from tower"
                  << std::endl;
        mutex_.unlock();
        boost::this_thread::sleep_for(
            boost::chrono::microseconds(std::rand() % 20));
      } while (!receiveMsg.Permission);
      /*{
              boost::unique_lock<boost::mutex> scoped_lock(mutex_);
              std::cout << tailNumber_ << " !TESTPOINT!" << std::endl;
      }*/
      if (leaving_)
      {
        boost::unique_lock<boost::mutex> scoped_lock(mutex_);
        currentRunway_ = &receiveMsg.RW;
        std::cout << tailNumber_ << " is taxiing to runway"
                  << currentRunway_->getRunwayNumber() << std::endl;
      }
      else
      {
        boost::unique_lock<boost::mutex> scoped_lock(mutex_);
        std::cout << tailNumber_ << " is taxiing from runway"
                  << currentRunway_->getRunwayNumber() << std::endl;
      }
    }
    catch (const interprocess_exception &ex)
    {
      std::cout << ex.what() << " At function ts_taxi()" << std::endl;
      exit(EXIT_FAILURE);
    }
  };

  std::vector<passenger> &getPassengers() { return passengers_; };

  std::string getTailNr() const { return tailNumber_; };

  void setPosition(std::string position) { position_ = position; };

  void changeLeaving() { leaving_ = true; };

  std::string getPosition() const { return position_; };

  bool isLeaving() const { return leaving_; };

  friend std::ostream &operator<<(std::ostream &os, const airplane &ap)
  {
    // boost::unique_lock<boost::mutex> scoped_lock(mutex_);
    os << "Tail Number: " << ap.tailNumber_ << "\n\tPosition: " << ap.position_
       << "\n\tCargo Capacity: " << ap.getCargoCapacity()
       << "\n\tPassenger Capacity: " << ap.getPassengerCapacity()
       << "\n\tPlane size: " << ap.getSize()
       << "\n\tReach of Plane: " << ap.getReach();
    return os;
  };
};

#endif /*_AIRPLANE_*/