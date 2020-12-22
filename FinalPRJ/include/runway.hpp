/*
 * Filename: /home/stud/APK/FinalPRJ/include/runway.hpp
 * Path: /home/stud/APK/FinalPRJ/include
 * Created Date: Monday, November 23rd 2020, 3:44:34 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 *
 * Copyright (c) 2020 APK - Group 4
 */
#ifndef _RUNWAY_
#define _RUNWAY_
#include <iostream>
class runway
{
public:
  runway() : number_(0), capacity_(0), occupied_(true){};
  runway(int RunwayNumber, int RunwayCapacity)
      : number_(RunwayNumber), capacity_(RunwayCapacity), occupied_(false){};
  ~runway(){};
  bool isOccupied() const { return occupied_; };
  void setOccupied(bool state) { occupied_ = state; };
  // void* getRunway() { return this; };
  int getRunwayNumber() const { return number_; };
  int getCapacity() const { return capacity_; };

private:
  int  number_;
  int  capacity_;
  bool occupied_;
};
#endif // !_RUNWAY_