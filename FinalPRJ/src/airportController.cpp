/*
 * Filename: /home/stud/APK/FinalPRJ/src/airportController.cpp
 * Path: /home/stud/APK/FinalPRJ/src
 * Created Date: Monday, November 23rd 2020, 3:48:15 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 *
 * Copyright (c) 2020 APK - Group 4
 */
#include "airplane.hpp"
#include "bagage.hpp"
#include "passenger.hpp"
#include <iostream>
int main()
{
  airplane<1, 10, 100, 1000> Airbus("Air", "MH370");
  airplane<2, 20, 200, 2000> Boeing("Runway_1", "SAS2");

  std::cout << Airbus << "\n\n" << Boeing << std::endl;
  bagage b(10, 100);
  std::cout << "\nBagage ID: " << b.getId()
            << "\n\tBagage Weight: " << b.getWeight() << std::endl;

  passenger<53213, 10> DinMor("Lolland's Mor");
  std::cout << "\nName: " << DinMor.getName() 
            << "\n\tID: " << DinMor.getId()
            << "\n\tBagage ID: " << DinMor.getBagageId() 
            << "\n\tBagage Weight: " << DinMor.getBagageWeight() 
            << std::endl;

  return 0;
}