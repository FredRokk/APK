#ifndef _EVENT_SIGNALS_HPP_
#define _EVENT_SIGNALS_HPP_

#include <mutex>
#include <boost/signals2/signal.hpp>

std::mutex mut;

unsigned int smphr = 0;

void give()
{
    std::unique_lock<std::mutex> lck(mut);
    smphr++;
};

void take()
{
    std::unique_lock<std::mutex> lck(mut);
    if(smphr > 0)
    {
        smphr--;
    }
}

unsigned int getWait()
{
    std::unique_lock<std::mutex> lck(mut);
    return smphr;
};

boost::signals2::signal<void (void)> AirportControlllerWaitSignal;

#endif /*_EVENT_SIGNALS_HPP_*/