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
  //const int cargoCapacity_, passengerCapacity_, size_, reach_;

public:
  planeProperties()
  {
      static_assert(Size <= 1500, "Capacity/Size exceeds absolute maximum");
  };
  ~planeProperties(){};
  int getCargoCapacity() const { return CargoCapacity; };
  int getPassengerCapacity() const { return PassengerCapacity; };
  int getReach() const { return Reach; };
  int getSize() const { return Size; };
};

#endif /*_PLANEPROPERTIES_*/