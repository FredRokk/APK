#ifndef TIMER_HPP_
#define TIMER_HPP_
#include <chrono>
#include <functional>
#include <map>
#include <string>
#include <thread>

class Timer
{
public:
  explicit Timer(int timer = 10);

  ~Timer();

private:
  int timerThreadFunction();

private:
  int          eventTimer_;
  std::thread *thread_;
  bool         terminator_;
};

#endif
