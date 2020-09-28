#include <boost/variant.hpp>
#include <iostream>

struct outputter_functor : public boost::static_visitor<>
{
  template <typename T> void operator()(T t) const
  {
    std::out << t << std::endl;
  }
}output;

