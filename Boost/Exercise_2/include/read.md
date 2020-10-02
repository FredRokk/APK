# This file is only for git pushing
*Also* **This** is a ***Markdown*** demo
## Code Preview
```c++
#include <boost/type_index.hpp>
#include <boost/variant.hpp>
#include <iostream>
#include <string>
#include <vector>

class FunctorClass : public boost::static_visitor<>
{
private:
  /* data */
public:
  template <typename T> void operator()(T t) const
  {
    std::cout << boost::typeindex::type_id<T>().pretty_name() << ' ' << t
              << std::endl;
  }
};

int main()
{
  FunctorClass visitor;
  std::vector<boost::variant<int, std::string, double, char>> vec;

  vec.push_back("Hello, World!");
  vec.push_back(420.69);
  vec.push_back(42);
  vec.push_back('F');

  std::for_each(vec.begin(), vec.end(), boost::apply_visitor(visitor));
}
```