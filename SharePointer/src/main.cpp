#include <iostream>
#include "SharedPtr.hpp"

int main()
{
    int val1 = 255;
    std::cout << "Hello, World" << std::endl;
    SharedPtr<int> ptr1(int(val1));

    std::cout << ptr1.count() << std::endl;
    return 0;
}

