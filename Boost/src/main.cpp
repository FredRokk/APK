#include <boost/variant.hpp>
#include <string>
#include <iostream>

int main()
{
    boost::variant<int, std::string, double> v;
    v = 255;
    std::cout << boost::get<int>(v) << std::endl;
    v = "Hello, World!";
    std::cout << boost::get<std::string>(v) << std::endl;
    v = 3.1415;
    std::cout << boost::get<double>(v) << std::endl;
    return 0;
}
