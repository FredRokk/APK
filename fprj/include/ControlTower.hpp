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

    void SendPermission()
    {
        SearchForEmptyRunway();
        if (chosenRunway_ != nullptr)
        {
            Messages::ControlTowerToPlane *message = new Messages::ControlTowerToPlane;
            message->permission = true;
            message->rwNumber = chosenRunway_->RunwayID;
            message_queue mq(open_or_create, "TowerToPlane", 100, sizeof(Messages::ControlTowerToPlane));
            mq.send(message, sizeof(Messages::ControlTowerToPlane), 0);
            chosenRunway_->occupied = true;
            chosenRunway_=nullptr;
        }
    };
    void ReceivePlaneHasLeft();
    void SendPlaneHasLeft();

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