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
#include <iostream>
#include <string>
class passenger
{
private:
  std::string name_;
  std::string destination_;
  int         id_;
  bagage      bagage_;

public:
  explicit passenger(std::string name, std::string destination, int ID,
                     int bagageWeight)
      : name_(name),
        id_(ID),
        bagage_(ID, bagageWeight),
        destination_(destination){};
  ~passenger(){};
  void        receiveBagage(){/*To be coded*/};
  void        giveBagage(){/*To be coded*/};
  void        setBagageWeight(int weight) { bagage_.setWeight(weight); };
  void        setBagageId() { bagage_.setId(id_); };
  std::string getName() const { return name_; };
  std::string getDestination() const { return destination_; };
  int         getId() const { return id_; };
  int         getBagageId() const { return bagage_.getId(); };
  int         getBagageWeight() const { return bagage_.getWeight(); };
  friend std::ostream &operator<<(std::ostream &os, const passenger &pass)
  {
    os << "\nName: " << pass.name_ << "\n\tDestination: " << pass.destination_
       << "\n\tID: " << pass.id_ << "\n\tBagage ID: " << pass.getBagageId()
       << "\n\tBagage Weight: " << pass.getBagageWeight();
  };
  friend passenger &operator>>(passenger &inPass, passenger &outPass)
  {
    inPass >> outPass.name_ >> outPass.id_ >> outPass.destination_;
    outPass.setBagageId();
    return inPass;
  };
};
#endif /*_PASSENGER_*/