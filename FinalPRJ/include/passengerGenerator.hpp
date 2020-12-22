/*
 * Filename: /home/stud/APK/FinalPRJ/include/passengerGenerator.hpp
 * Path: /home/stud/APK/FinalPRJ/include
 * Created Date: Monday, November 23rd 2020, 3:43:52 pm
 * Author: Frederik Rokkjær, Frederik Kronvang og Christian Olsen
 *
 * Copyright (c) 2020 APK - Group 4
 */
constexpr auto DEBUG = 0;

#ifndef _PASSENGERGENERATOR_
#define _PASSENGERGENERATOR_

#include "passenger.hpp"
#include <cstdlib>
#include <ctime>
#include <string>

class passengerGenerator
{
private:
  std::string destination[5] = {"Egypt", "England", "USA", "Russia", "Germany"};
  std::string firstName[20]  = {
      "William ", "Alfred ", "Oscar ",  "Noah ", "Karl ",   "Lucas ", "Oliver ",
      "Arthur ",  "August ", "Malthe ", "Emma ", "Alma ",   "Clara ", "Freja ",
      "Sofia ",   "Karla ",  "Agnes ",  "Ella ", "Olivia ", "Anna "};
  std::string surname[20] = {
      "Jensen",      "Nielsen", "Hansen",    "Pedersen", "Andersen",
      "Christensen", "Larsen",  "Rasmussen", "Petersen", "Madsen",
      "Kristensen",  "Olsen",   "Thomsen",   "Poulsen",  "Johansen",
      "Knudsen",     "Schmidt", "Rokkjaer",  "Kronvang", "Gade"};
  int id_;

public:
  passengerGenerator(/* args */) : id_(0) { std::srand(std::time(NULL)); };
  ~passengerGenerator(){};
  passenger generatePassenger(int minWeight, int maxWeight)
  {
    id_++;
    std::string tempName =
        firstName[std::rand() % 20] + surname[std::rand() % 20];
    passenger tempPassenger(tempName, destination[std::rand() % 5], id_,
                            minWeight +
                                std::rand() % ((maxWeight + 1) - minWeight));
    return tempPassenger;
  };
  void transformPassenger(passenger &pass, int minWeight, int maxWeight)
  {
    id_++;
    std::string tempName =
        firstName[std::rand() % 20] + surname[std::rand() % 20];
    passenger tempPassenger(tempName, destination[std::rand() % 5], id_,
                            std::rand() % ((maxWeight + 1) - minWeight));
    if constexpr (DEBUG)
    {
      std::cout << "Passenger within transform " << std::endl;
      std::cout << "###################################" << std::endl;
      std::cout << tempPassenger << std::endl;
    }

    pass = std::move(tempPassenger);

    if constexpr (DEBUG)
    {
      std::cout << "###################################" << std::endl;
    }
  }
};

#endif /*_PASSENGERGENERATOR_*/