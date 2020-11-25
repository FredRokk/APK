/*
 * Filename: /home/stud/APK/FinalPRJ/include/passenger.hpp
 * Path: /home/stud/APK/FinalPRJ/include
 * Created Date: Monday, November 23rd 2020, 3:45:58 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 *
 * Copyright (c) 2020 APK - Group 4
 */

/*
- name_ : string
- id_ : int
- bagageId_ : int
- bagageWeight_ : int

+ osteam overload
+ getBagage() : void
+ giveBagage() : void
*/
#ifndef _PASSENGER_
#define _PASSENGER_
#include "bagage.hpp"
#include <string>
template <int ID, int BagageWeight> class passenger
{
private:
  const std::string name_, destination_;
  const int         id_;
  bagage            bagage_;

public:
  passenger(std::string name, std::string destination)
      : name_(name), id_(ID), bagage_(ID, BagageWeight), destination_(destination);{};
  ~passenger(){};
  void        receiveBagage(){/*To be coded*/};
  void        giveBagage(){/*To be coded*/};
  std::string getName() const { return name_; };
  const int   getId() const { return id_; };
  const int   getBagageId() const { return bagage_.getId(); };
  const int   getBagageWeight() const { return bagage_.getWeight(); };
};
#endif /*_PASSENGER_*/