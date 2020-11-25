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
#include "bagage.hpp"
#include <string>
template <int ID, int BagageId, int BagageWeight> class passenger
{
private:
  const std::string name_;
  const int         id_;
  bagage            bagage_;

public:
  passenger(std::string name)
      : name_(name), id_(ID), bagage_(BagageId, BagageWeight){};
  ~passenger(){};
  void getBagage(){};
  void giveBagage(){};
};