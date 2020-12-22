/*
 * Filename: /home/stud/APK/FinalPRJ/include/gate.hpp
 * Path: /home/stud/APK/FinalPRJ/include
 * Created Date: Monday, November 23rd 2020, 3:45:44 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 *
 * Copyright (c) 2020 APK - Group 4
 */
#ifndef _GATE_
#define _GATE_

#include "airplane.hpp"
#include <algorithm>
#include <vector>
//#include "passenger.hpp"

class gate
{
public:
  gate(int GateNumber) : number_(GateNumber), occupied_(false){};
  ~gate(){};
  template <int CargoCapacity, int PassengerCapacity, int Size, int Reach>
  void
  emptyAirplane(airplane<CargoCapacity, PassengerCapacity, Size, Reach> &plane,
                std::vector<passenger> &                                 pass)
  {
    std::vector<passenger> tempVec;
    tempVec.swap(plane.getPassengers());
    pass.insert(pass.end(), std::make_move_iterator(tempVec.begin()),
                std::make_move_iterator(tempVec.end()));
  };

  template <int CargoCapacity, int PassengerCapacity, int Size, int Reach>
  void
  fillAirplane(airplane<CargoCapacity, PassengerCapacity, Size, Reach> &plane,
               std::vector<passenger> &                                 pass)
  {
    std::vector<passenger> &tempVec = plane.getPassengers();
    if (tempVec.size() >= PassengerCapacity)
    {
      tempVec.swap(pass);
    }
    else
    {
      for (size_t i = 0; i < PassengerCapacity; i++)
      {
        tempVec.push_back(std::move(pass[0]));
        pass.erase(pass.begin());
      }
    }
  };

  bool isOccupied() const { return occupied_; };
  void setOccupied(bool state) { occupied_ = state; };

private:
  const int number_;
  bool      occupied_;
};
#endif // !_GATE_