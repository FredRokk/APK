#ifndef _CONTROL_TOWER_HPP_
#define _CONTROL_TOWER_HPP_

#include "Airplane.hpp"
#include "Messages.hpp"
#include <algorithm>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <list>
#include <thread>

class ControlTower
{
private:
  /* data */
  struct Runway_
  {
    const int RunwayID;
    bool      occupied;
  };
  std::list<Runway_ *> runways_;
  Runway_ *            chosenRunway_ = nullptr;
  void                 SearchForEmptyRunway()
  {
    for (auto &rwItem : runways_)
    {
      if (rwItem->occupied == 0)
      {
        chosenRunway_ = rwItem;
        break;
      }
    }
  };

  void SearchForRunway(int rwNumber)
  {
    for (auto &rwItem : runways_)
    {
      if (rwItem->RunwayID == rwNumber)
      {
        chosenRunway_ = rwItem;
        break;
      }
    }
  };

  // Using algorithm
  static bool ConditionForSearch(const Runway_ *rwItem, int n)
  {
    return rwItem->RunwayID == n;
  };
  void SearchForRunwayUsingAlgo(int rwNumber)
  {
    auto match = std::find_if(
        runways_.begin(), runways_.end(),
        std::bind(&ConditionForSearch, std::placeholders::_1, rwNumber));
    if (match != runways_.end())
    {
      chosenRunway_ = *match;
    }
  };

public:
  // Default Contructor
  ControlTower(int num_runways = 1)
  {
    for (int i = 0; i < num_runways; i++)
    {
      runways_.push_back(new Runway_{.RunwayID = i, .occupied = false});
    }
  };
  // Copy Contructor
  ControlTower(const ControlTower &otherTower)
  {
    runways_.clear();
    std::copy(otherTower.runways_.begin(), otherTower.runways_.end(),
              std::back_inserter(runways_));
    chosenRunway_ = otherTower.chosenRunway_;
  };
  // Move Contructor
  explicit ControlTower(const ControlTower &&otherTower) noexcept
  {
    runways_ = std::move(otherTower.runways_);
  };

  ~ControlTower()
  {
    for (auto &rwItem : runways_)
    {
      delete rwItem;
    }
    runways_.clear();
  };

  bool SendPermission(int planeId)
  {
    SearchForEmptyRunway();
    if (chosenRunway_ != nullptr)
    {
      Messages::ControlTowerToPlane message;
      message.permission = true;
      message.rwNumber   = chosenRunway_->RunwayID;

      std::string planeName = std::to_string(planeId);
      SendMessage<Messages::ControlTowerToPlane>(planeName, message);

      chosenRunway_->occupied = true;
      chosenRunway_           = nullptr;
      return true;
    }
    else
    {
      return false;
    }
  };
  void ReceiveMessageFromPlane()
  {
    bool recievedMessage = false;

    auto message = receiveMessage<Messages::PlaneToControltower>(
        "PlaneToTower", recievedMessage);
    if (recievedMessage)
    {
      if (message.RequestingPermission)
      {
        SendPermission(message.FlightID);
      }
      else if (message.HasLeft)
      {
        SearchForRunway(message.RunwayID);
        if (chosenRunway_ != nullptr)
        {
          chosenRunway_->occupied = false;
          chosenRunway_           = nullptr;
        }
        SendPlaneHasLeft(message.FlightID);
      }
      else
      {
        SearchForRunwayUsingAlgo(message.RunwayID);
        if (chosenRunway_ != nullptr)
        {
          chosenRunway_->occupied = false;
          chosenRunway_           = nullptr;
        }
      }
    }
  };
  void SendPlaneHasLeft(int &flightID)
  {
    Messages::PlaneHasLeft message;
    message.FlightID = flightID;

    SendMessage<Messages::PlaneHasLeft>("FromTowerToAirport", message);
  };

  ControlTower &operator=(const ControlTower &tower) // copy ass op
  {
    ControlTower copyTower(tower);
    std::swap(*this, copyTower);
    return *this;
  };

  ControlTower &operator=(ControlTower &&otherTower) noexcept // move ass op
  {
    if (this != &otherTower)
    {
      runways_ = std::move(otherTower.runways_);
    }
    else
    {
      operator=(otherTower);
    }
    return *this;
  };

  void operator()()
  {
    using namespace std::literals;
    while (true)
    {
      ReceiveMessageFromPlane();
      std::this_thread::sleep_for(500ms);
    }
  }
};

/* ----------  RULE OF 5 -----------------
    Contructor,
    Destructor,
    Copy Constructor,
    Move Constructor,
    Assignment operator (copy og move)
*/
#endif