#include <iostream>
#include "custom.h"

int main()
{
    int input;
    std::cin >>input;

    if (input == 0)
    {
        fun();      
    }
    else
    {
        std::cout << "Hello World" << std::endl;
    }

    return 0;
}
