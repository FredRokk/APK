/*
 * Filename: /home/stud/APK/FinalPRJ/include/controlTower.hpp
 * Path: /home/stud/APK/FinalPRJ/include
 * Created Date: Monday, November 23rd 2020, 3:44:18 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 *
 * Copyright (c) 2020 APK - Group 4
 */

#ifndef _CONTROLTOWER_
#define _CONRTOLTOWER_

//#include "airplane.hpp"
#include "runway.hpp"
#include <algorithm>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace boost::interprocess;

class controlTower
{
public:
  controlTower(std::vector<runway *> &runwayVec)
      : runways_(runwayVec), currentRunway_(nullptr){};
  ~controlTower(){};

  void ts_handleAirplane()
  {
    boost::unique_lock<boost::mutex> scoped_lock(mutex_);
    try
    {
      message_queue            planeToTower_(open_or_create, "tower_msgs", 100,
                                  sizeof(Messages::PlaneToTower));
      message_queue::size_type received_size;
      unsigned int             priority = 1;
      std::cout << "Waiting for package" << std::endl;
      planeToTower_.receive(&plane2towerPackage_, sizeof(plane2towerPackage_),
                            received_size, priority);
      std::cout << "Recieved package from plane " << plane2towerPackage_.TailNr
                << std::endl;
      currentRunway_ = &plane2towerPackage_.RW;
      std::cout << "Position: " << plane2towerPackage_.Position << std::endl;
      switch (plane2towerPackage_.Position)
      {
      case in_air:
        std::cout << "Case: in_air" << std::endl;
        if (plane2towerPackage_.RequestForLanding)
        {
          findAndSendRunwayViaMessageQueue();
        }
        else
        {
          currentRunway_ = &plane2towerPackage_.RW;
          currentRunway_->setOccupied(false);
        }
        break;
      case taking_off:
        std::cout << "Case: taking_off" << std::endl;
        plane2towerPackage_.RW.setOccupied(false);
        break;
      case entering_runway:
        std::cout << "Case: entering_runway" << std::endl;
        findAndSendRunwayViaMessageQueue();
        break;
      case exiting_runway:
      {
        std::cout << "Case: exiting_runway" << std::endl;
        message_queue tempMQ(open_only, plane2towerPackage_.TailNr.c_str());
        plane2towerPackage_.RW.setOccupied(false);
        tower2planePackage_.Permission = true;
        tempMQ.send(&tower2planePackage_, sizeof(tower2planePackage_), 0);
        break;
      }
      default:
        std::cout
            << "Tower: Received packet from plane: "
            << plane2towerPackage_.TailNr
            << " couldn't be understood, and therefore haven't been processed"
            << std::endl;
        break;
      }
    }
    catch (const interprocess_exception &ex)
    {
      message_queue::remove("tower_msgs");
      std::cout << ex.what() << " At function ts_handleAirplane()" << std::endl;
      exit(EXIT_FAILURE);
    }
  };

private:
  void searchForFreeRunway(std::vector<runway *> &RW, bool &permission,
                           runway **currentRunwayPtr)
  {
    for (auto itr = RW.begin(); itr != RW.end(); itr++)
    {
      if (!(*itr)->isOccupied() && !permission)
      {
        *currentRunwayPtr = &**itr; // Even more beautiful!!! Made by FBR
        permission        = true;
      }
    }
  };

  void findAndSendRunwayViaMessageQueue()
  {
    bool permissionToEnter_ = false;
    searchForFreeRunway(runways_, permissionToEnter_, &currentRunway_);
    message_queue tempMQ(open_only, plane2towerPackage_.TailNr.c_str());
    tower2planePackage_.Permission = permissionToEnter_;
    tower2planePackage_.RW         = *currentRunway_;
    currentRunway_->setOccupied(true);
    tempMQ.send(&tower2planePackage_, sizeof(tower2planePackage_), 0);
  };

  Messages::TowerToPlane tower2planePackage_;
  Messages::PlaneToTower plane2towerPackage_;
  std::vector<runway *>  runways_;
  boost::mutex           mutex_;
  runway *               currentRunway_;
};

#endif // !_CONTROLTOWER_