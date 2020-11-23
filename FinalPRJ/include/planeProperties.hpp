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

template <int Capacity, int Size> class planeProperties
{
private:
  int capacity_;
  int size_;

public:
  planeProperties() : capacity_(Capacity), size_(Size){};
  ~planeProperties(){};
  int getCapacity() const { return capacity_; };
  int getSize() const { return size_; };
};

#endif /*_PLANEPROPERTIES_*/