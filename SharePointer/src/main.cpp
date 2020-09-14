#include <iostream>
#include "SharedPtr.hpp"

int main()
{
    int val1 = 255;
    std::cout << "Hello, World" << std::endl;
    SharedPtr<int> ptr1(int(val1));
    SharedPtr<int> ptr2(ptr1);

    std::cout << ptr2.count() << std::endl;
    return 0;
}

