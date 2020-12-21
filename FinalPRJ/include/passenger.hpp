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
#include <string>
#include <utility>
class passenger
{
private:
	std::string name_;
	std::string destination_;
	int         id_;
	std::shared_ptr<bagage> bagage_;

public:
	passenger() // Default Constructor
		: name_("John Doe"), id_(0), bagage_(new bagage(0, 0)), destination_("Unknown") {};

	passenger(std::string name, std::string destination, int ID, int bagageWeight) // Parametric Contructor
		: name_(name),
		id_(ID),
		bagage_(new bagage(ID, bagageWeight)),
		destination_(destination) {};

	passenger(const passenger& otherPass) // Copy Contructor
		: name_(otherPass.name_),
		id_(otherPass.id_),
		bagage_(otherPass.bagage_),
		destination_(otherPass.destination_) {};

	explicit passenger(passenger&& otherPass) noexcept // Move Contructor
		: name_(""), id_(0), bagage_(new bagage(0, 0)), destination_("")
	{
		name_ = std::move(otherPass.name_);
		id_ = std::move(otherPass.id_);
		bagage_ = std::move(otherPass.bagage_);
		destination_ = std::move(otherPass.destination_);
	};

	~passenger()
	{
		bagage_.~shared_ptr();
	};

	//void        receivebagage() { /*to be coded*/ };
	//void        givebagage() {/*to be coded*/ };
	void        setBagageWeight(int weight) { bagage_->setWeight(weight); };
	void        setBagageId() { bagage_->setId(id_); };
	std::string getName() const { return name_; };
	std::string getDestination() const { return destination_; };
	int         getId() const { return id_; };
	int         getBagageId() const { return bagage_->getId(); };
	int         getBagageWeight() const { return bagage_->getWeight(); };
	friend std::ostream& operator<<(std::ostream& os, const passenger& pass)
	{
		os << "Name: " << pass.name_ << "\n\tDestination: " << pass.destination_
			<< "\n\tID: " << pass.id_ << "\n\tBagage ID: " << pass.getBagageId()
			<< "\n\tBagage Weight: " << pass.getBagageWeight();
		return os;
	};
	passenger& operator=(const passenger& pass) // Copy Assignment operator
	{
		passenger copyPass(pass);
		std::swap(*this, copyPass);
		return *this;
	};
	passenger& operator=(passenger&& otherPass) noexcept // Move Assignment operator
	{
		if (this != &otherPass)
		{
			name_ = std::move(otherPass.name_);
			id_ = std::move(otherPass.id_);
			destination_ = std::move(otherPass.destination_);
			bagage_ = std::move(otherPass.bagage_);
			return *this;
		}
	};
};
#endif /*_PASSENGER_*/