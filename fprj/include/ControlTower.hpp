#ifndef _CONTROL_TOWER_HPP_
#define _CONTROL_TOWER_HPP_

#include "Airplane.hpp"
#include "Messages.hpp"
#include <algorithm>
#include <list>
#include <boost/interprocess/ipc/message_queue.hpp> 

class ControlTower
{
private:
    /* data */
    struct Runway_
    {
        int RunwayID;
        bool occupied;
    };
    std::list<Runway_*> runways_;
    Runway_* chosenRunway_ = nullptr;
    void SearchForEmptyRunway()
    {
        for (auto &rwItem : runways_)
        {
            if (!rwItem->occupied)
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
    static bool ConditionForSearch(const Runway_& rwItem, int n)
    {
      return rwItem.RunwayID == n;
    };
    void SearchForRunwayUsingAlgo(int rwNumber)
    {
        auto match = std::find_if(runways_.begin(), runways_.end(), std::bind(&ConditionForSearch, std::placeholders::_1, rwNumber));
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
            runways_.push_back(new Runway_{
                .RunwayID = i, 
                .occupied = false});
        }
    };
    // Copy Contructor
    ControlTower(const ControlTower &otherTower) : runways_(otherTower.runways_)
    {};
    // Move Contructor
    explicit ControlTower(const ControlTower &&otherTower) noexcept
    {
        runways_ = std::move(otherTower.runways_);
    };
    
    ~ControlTower()
    {
        for (auto& rwItem : runways_)
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
            message.rwNumber = chosenRunway_-> RunwayID;
            std::string planeName = std::to_string(planeId);
            message_queue mq(open_or_create, planeName.c_str(), 100, sizeof(Messages::ControlTowerToPlane));
            mq.send(&message, sizeof(Messages::ControlTowerToPlane), 0);
            chosenRunway_ -> occupied = true;
            chosenRunway_ = nullptr;
            return true;
        }
        else
        {
            return false;
        }
    };
    void ReceiveMessageFromPlane()
    {   
        message_queue::size_type recvd_size;
        unsigned int             priority = 0;
        bool recievedMessage = false;
        message_queue mq(open_or_create, "PlaneToTower", 100, sizeof(Messages::PlaneToControltower));
        Messages::PlaneToControltower *message = new Messages::PlaneToControltower;
        recievedMessage = mq.try_receive(message, sizeof(Messages::PlaneToControltower), recvd_size, priority);
        if(recievedMessage)
        {
          if (message->RequestingPermission)
          {
            bool success = false;
            while (!success)
            {
              success = SendPermission(message->FlightID);
            }
          }
          else if (message->HasLeft)
          {
              SendPlaneHasLeft(message->FlightID);
          }
          else
          {
            SearchForRunwayUsingAlgo(message->RunwayID);
            if (chosenRunway_ != nullptr)
            {
              chosenRunway_->occupied = false;
              chosenRunway_           = nullptr;
            }
          }
          delete message;
        }
    };
    void SendPlaneHasLeft(int &flightID)
    {
        Messages::PlaneHasLeft message;
        message_queue mq(open_or_create, "FromTowerToAirport", 100, sizeof(Messages::PlaneHasLeft));
        message.FlightID = flightID;
        
        mq.send(&message, sizeof(Messages::PlaneHasLeft), 0);
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
};

/* ----------  RULE OF 5 -----------------
    Contructor,
    Destructor,
    Copy Constructor,
    Move Constructor,
    Assignment operator (copy og move)
*/
#endif