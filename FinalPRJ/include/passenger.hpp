/*
 * Filename: /home/stud/APK/FinalPRJ/include/passenger.hpp
 * Path: /home/stud/APK/FinalPRJ/include
 * Created Date: Monday, November 23rd 2020, 3:45:58 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 *
 * Copyright (c) 2020 APK - Group 4
 */

#ifndef _PASSENGER_
#define _PASSENGER_
#include "bagage.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <memory_resource>
#include <string>
#include <utility>

class passenger
{
  using allocator = std::pmr::polymorphic_allocator<std::byte>;

private:
  std::pmr::string        name_;
  std::pmr::string        destination_;
  int                     id_;
  int                     bagageWeight_;  
  int                     bagageID_;
  std::shared_ptr<bagage> bagage_;
  allocator               alloc_;

public:
  passenger() // Default Constructor
      : name_("John Doe"),
        id_(0),
        bagage_(new bagage(0, 0)),
        destination_("Unknown"){};

  passenger(std::pmr::string name, std::pmr::string destination, int ID,
            int       bagageWeight,
            allocator alloc =
                std::pmr::get_default_resource()) // Parametric Contructor
      : name_(name),
        id_(ID),
        bagage_(new bagage(ID, bagageWeight)),
        destination_(destination),
        alloc_(alloc){};

  passenger(const passenger &otherPass) // Copy Contructor
      : name_(otherPass.name_),
        id_(otherPass.id_),
        bagage_(otherPass.bagage_),
        destination_(otherPass.destination_),
        alloc_(otherPass.alloc_){};

  passenger(const passenger &otherPass,
            allocator        alloc) // Alloc Copy Constructor
      : name_(otherPass.name_),
        id_(otherPass.id_),
        bagage_(otherPass.bagage_),
        destination_(otherPass.destination_),
        alloc_(alloc){};

  explicit passenger(passenger &&otherPass) noexcept // Move Contructor
      : name_(""), id_(0), bagage_(new bagage(0, 0)), destination_("")
  {
    name_        = std::move(otherPass.name_);
    id_          = std::move(otherPass.id_);
    bagage_      = std::move(otherPass.bagage_);
    destination_ = std::move(otherPass.destination_);
  };

  passenger(passenger &&otherPass, allocator alloc) noexcept : alloc_(alloc)
  {
    operator=(std::move(otherPass));
  };

  ~passenger() { bagage_.~shared_ptr(); };

  // void        receivebagage() { /*to be coded*/ };
  // void        givebagage() {/*to be coded*/ };
  void             setBagageWeight(int weight) { bagage_->setWeight(weight); };
  void             setBagageId() { bagage_->setId(id_); };
  std::pmr::string getName() const { return name_; };
  std::pmr::string getDestination() const { return destination_; };
  int              getId() const { return id_; };
  int              getBagageId() const { return bagage_->getId(); };
  int              getBagageWeight() const { return bagage_->getWeight(); };
  friend std::ostream &operator<<(std::ostream &os, const passenger &pass)
  {
    os << "Name: " << pass.name_ << "\n\tDestination: " << pass.destination_
       << "\n\tID: " << pass.id_ << "\n\tBagage ID: " << pass.getBagageId()
       << "\n\tBagage Weight: " << pass.getBagageWeight();
    return os;
  };

  friend std::istream &operator>>(std::istream &is, passenger &pass)
  {
    is >> pass.name_ >> pass.destination_ >> pass.id_ >> pass.bagageID_ >> pass.bagageWeight_;
    pass.setBagageWeight(pass.bagageWeight_);
    pass.setBagageId();
    return is;
  };

  passenger &operator=(const passenger &pass) // Copy Assignment operator
  {
    passenger copyPass(pass);
    std::swap(*this, copyPass);
    return *this;
  };
  passenger &
  operator=(passenger &&otherPass) noexcept // Move Assignment operator
  {
    // if (this != &otherPass)
    // {
    //   name_        = std::move(otherPass.name_);
    //   id_          = std::move(otherPass.id_);
    //   destination_ = std::move(otherPass.destination_);
    //   bagage_      = std::move(otherPass.bagage_);
    // }
    // return *this;

    if (alloc_ == otherPass.alloc_)
    {
      std::swap(name_, otherPass.name_);
      std::swap(destination_, otherPass.destination_);
      std::swap(id_, otherPass.id_);
      std::swap(bagage_, otherPass.bagage_);
    }
    else
    {
      operator=(otherPass);
    }
    return *this;
  };

  allocator getAllocator() const { return alloc_; };
};
#endif /*_PASSENGER_*/