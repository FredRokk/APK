#ifndef _TIMER_HPP_
#define _TIMER_HPP_
#ifdef DEBUG

#include <chrono>
#include <iostream>

class timer
{
private:
    std::chrono::_V2::system_clock::time_point start;
    std::chrono::_V2::system_clock::time_point end;
public:

    timer()
    {
        start = std::chrono::high_resolution_clock::now();
    };
    
    ~timer()
    {
        end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Elapsed time: " << elapsed_time.count() << std::endl;
    };
};

#else
class timer
{
private:
public:
    timer();
    ~timer();
};

#endif /*DEBUG*/
#endif /*_TIMER_HPP_*/