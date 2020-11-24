/*
 * Filename: /home/stud/APK/FinalPRJ/include/planeType.hpp
 * Path: /home/stud/APK/FinalPRJ/include
 * Created Date: Monday, November 23rd 2020, 3:42:40 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 *
 * Copyright (c) 2020 APK - Group 4
 */

#ifndef _PLANEPROPERTIES_
#define _PLANEPROPERTIES_
template <int CargoCapacity, int PassengerCapacity, int Size, int Reach>
class planeProperties
{
private:
  int cargoCapacity_, passengerCapacity_, size_, reach_;

public:
  planeProperties()
      : cargoCapacity_(CargoCapacity),
        passengerCapacity_(PassengerCapacity),
        size_(Size), reach_(Reach){};
  ~planeProperties(){};
  int getCargoCapacity() const { return cargoCapacity_; };
  int getPassengerCapacity() const { return passengerCapacity_; };
  int getReach() const { return reach_; };
  int getSize() const { return size_; };
};

#endif /*_PLANEPROPERTIES_*/