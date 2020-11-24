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
#include <string>
#include <vector>
template <int CargoCapacity, int PassengerCapacity, int Size, int Reach>
class airplane
    : public planeProperties<CargoCapacity, PassengerCapacity, Size, Reach>
{
private:
  // int tailNumber_;
  // std::vector<passenger> passengers_;
  // std::string destination_, position_;
  // planeProperties::planeProperties planeType_;
public:
  airplane(){};
  ~airplane() {};
  bool takeOff(){};
  bool touchDown(){};
  bool taxi(){};
  // void fill(std::vector<passenger> passengers);
  // void unload(std::vector<passenger> passengers);
  std::string getPosition() const {};
};

#endif /*_AIRPLANE_*/