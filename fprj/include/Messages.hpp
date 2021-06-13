#ifndef _MESSAGES_HPP_
#define _MESSAGES_HPP_
#include <string>
#include <boost/interprocess/ipc/message_queue.hpp> 
#include <iostream>
#include <mutex>
#include <thread>
#include "Destination.hpp"
#include "Sizes.hpp"

std::mutex SendMtx;
std::mutex ReceiveMtx;


class Message
{
public:
    virtual ~Message() = default;
};

namespace Messages 
{
    struct GateToAirportController : public Message
    {
        void *GateAddr;
    };

    struct ControlTowerToPlane : public Message
    {
        bool permission;
        int  rwNumber;
    };

    struct PassengerToAirportController : public Message
    {
        Destination Destination_;
        int PassengerID_;
    };

    struct AirportControllerToPassenger : public Message
    {
        int GateNumber;
    };

    struct PlaneToControltower : public Message
    {
        int FlightID;
        bool RequestingPermission;
        bool HasLeft;
        int RunwayID;
    };

    struct PlaneConfirmation : public Message
    {
        int GateID;
        int FlightID;
    };
    
    struct PlaneHasLeft : public Message
    {
        int FlightID;
    };

     struct AirportControllerToAirplane : public Message
    {
        Destination Destination_;
        int GateNumber;
    };

    struct planeInfo : public Message
    {
        void *planeAddr;
    };

    struct passengerInfo : public Message
    {
        void *passengerAddr;
    };
}
using namespace boost::interprocess;
using namespace std::literals;
template<typename T>
T receiveMessage(std::string messageQueueName)
{
    size_t recvd_size;
    unsigned int priority = 0;
    bool recvd_msg = false;
    T *msg = new T;
    do
    {
      ReceiveMtx.lock();
      try
      {
        message_queue mq(open_or_create, messageQueueName.c_str(), 100,
                         sizeof(T));
        recvd_msg = mq.try_receive(msg, sizeof(T), recvd_size, priority);
      }
      catch (const interprocess_exception &ex)
      {
        message_queue::remove(messageQueueName.c_str());
        std::cout << ex.what() << " receiveMessage: \"" << messageQueueName << "\"" << std::endl;
        exit(EXIT_FAILURE);
      }
      ReceiveMtx.unlock();
      std::this_thread::sleep_for(1ms);
    }while (!recvd_msg);    
    
    return *msg;
}

template<typename T>
T receiveMessage(std::string messageQueueName, bool &msg_recvd)
{
    ReceiveMtx.lock();
    T *msg = new T;
    try
    {
        size_t       recvd_size;
        unsigned int priority = 0;

        message_queue mq(open_or_create, messageQueueName.c_str(), 100,
                        sizeof(T));
        msg_recvd = mq.try_receive(msg, sizeof(T), recvd_size, priority);
    }
    catch (const interprocess_exception &ex)
    {
        message_queue::remove(messageQueueName.c_str());
        std::cout << ex.what() << " receiveMessage: \"" << messageQueueName << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
    ReceiveMtx.unlock();
    if (msg_recvd)
    {
        return *msg;
    }
}

template<typename T>
void SendMessage(std::string messageQueueName, T msg)
{
    SendMtx.lock();
    unsigned int priority = 0;
    try
    {
        message_queue mq(open_or_create, messageQueueName.c_str(), 100, sizeof(T));
        //std::cout << "Opened or created message queue \"" << messageQueueName << "\"" << std::endl;
        mq.send(&msg, sizeof(T), priority);
        //std::cout << " send message " << messageQueueName << std::endl;
    }
    catch (const interprocess_exception &ex)
    {
      message_queue::remove(messageQueueName.c_str());
      std::cout << ex.what() << " SendMessage: \"" << messageQueueName << "\"" << std::endl;
      exit(EXIT_FAILURE);
    }
    SendMtx.unlock();
}
//message_queue fromPlaneToTower_(open_or_create, "tower_msgs", 100, sizeof(Messages::PlaneToTower));

#endif /*_MESSAGES_HPP_*/
        
    