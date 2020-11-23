/*
 * Filename: /home/stud/APK/FinalPRJ/src/airportController.cpp
 * Path: /home/stud/APK/FinalPRJ/src
 * Created Date: Monday, November 23rd 2020, 3:48:15 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 * 
 * Copyright (c) 2020 APK - Group 4
 */
#include "planeProperties.hpp"
#include <iostream>

int main()
{
    planeProperties<200, 1500> Boing737;
    std::cout << Boing737.getSize() << "," << Boing737.getCapacity() << std::endl;
    return 0;
}