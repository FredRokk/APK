#include <boost/variant.hpp>
#include <iostream>
#include <string>

template <typename T>
std::ostream &operator<<(std::ostream &os, const boost::variant<T> &var)
{
  os << boost::get<T>(var);
  return os;
}

int main()
{
  boost::variant<int, std::string, double> v;
  v = 255;
  std::cout << v << std::endl;
  v = "Hello, World!";
  std::cout << v << std::endl;
  v = 3.1415;
  std::cout << v << std::endl;

  return 0;
}
