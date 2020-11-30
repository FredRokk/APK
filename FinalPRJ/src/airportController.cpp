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
#include "passengerGenerator.hpp"
#include <iostream>
int main()
{
  airplane<1, 10, 100, 1000> Airbus("Air", "MH370");
  airplane<2, 20, 200, 2000> Boeing("Runway_1", "SAS2");

  std::cout << Airbus << "\n\n" << Boeing << std::endl;
  bagage b(10, 100);
  std::cout << "\nBagage ID: " << b.getId()
            << "\n\tBagage Weight: " << b.getWeight() << std::endl;

  passenger DinMor("Lolland's Mor", "Olsens hjem ( ͡° ͜ʖ ͡°)", 420, 69);
  std::cout << "\nName: " << DinMor.getName()
            << "\n\tDestination: " << DinMor.getDestination() 
            << "\n\tID: " << DinMor.getId()
            << "\n\tBagage ID: " << DinMor.getBagageId() 
            << "\n\tBagage Weight: " << DinMor.getBagageWeight() 
            << std::endl;
  passengerGenerator passGen;

  passenger test1 = passGen.generatePassenger(1, 15);
  std::cout << test1 << std::endl;

  passenger test2 = passGen.generatePassenger(1, 15);
  std::cout << test2 << std::endl;

  passGen.transformPassenger(test2, 1, 15);
  std::cout << test2 << std::endl;
  
  return 0;
}