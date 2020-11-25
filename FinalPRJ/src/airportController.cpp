/*
 * Filename: /home/stud/APK/FinalPRJ/src/airportController.cpp
 * Path: /home/stud/APK/FinalPRJ/src
 * Created Date: Monday, November 23rd 2020, 3:48:15 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 *
 * Copyright (c) 2020 APK - Group 4
 */
#include "airplane.hpp"
#include <iostream>
int main()
{
  airplane<1, 10, 100, 1000> Airbus("Air", "MH370");
  airplane<2, 20, 200, 2000> Boeing("Rail1", "SAS2");

  std::cout << Airbus << "\n\n" << Boeing <<std::endl;
  return 0;
}