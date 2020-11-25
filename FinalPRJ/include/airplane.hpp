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
#include "passenger.hpp"
#include "planeProperties.hpp"
#include <iostream>
#include <string>
#include <vector>
template <int CargoCapacity, int PassengerCapacity, int Size, int Reach>
class airplane
    : public planeProperties<CargoCapacity, PassengerCapacity, Size, Reach>
{
private:
  const std::string tailNumber_;
  std::vector<passenger> passengers_;
  std::string destination_, position_;
  bool isTaxi_, isTakingOff_, isTouchingDown_;

public:
  airplane(std::string Position, std::string TailNumber)
      : position_(Position), tailNumber_(TailNumber){};
  ~airplane(){};
  bool takeOff(){};
  bool touchDown(){};
  bool taxi(){};
  void fill(std::vector<passenger> passengers){/*To be coded*/};
  void unload(std::vector<passenger> passengers){/*To be coded*/};
  void        setPosition(std::string position) { position_ = position; };
  std::string getPosition() const { return position_; };
  friend std::ostream &operator<<(std::ostream &os, const airplane &ap)
  {
    os << "Tail Number: " << ap.tailNumber_ << "\n\tPosition: " << ap.position_
       << "\n\tCargo Capacity: " << ap.getCargoCapacity()
       << "\n\tPassenger Capacity: " << ap.getPassengerCapacity()
       << "\n\tPlane size: " << ap.getSize()
       << "\n\tReach of Plane: " << ap.getReach();
    return os;
  };
};

#endif /*_AIRPLANE_*/