#include <iostream>
#include "src/circular_buffer.hpp"
#include <vector>
int main()
{
    circular_buffer<int> circularBuffer(3);
    std::cout << "Empty: " << (circularBuffer.empty() ? "true" : "false") << std::endl;

    circularBuffer.push_back(1);
    circularBuffer.push_back(2);
    circularBuffer.push_back(3);

    std::cout << circularBuffer.front() << std::endl;
    std::cout << circularBuffer.back() << std::endl;
    std::cout << "Full: " << (circularBuffer.full() ? "true" : "false") << std::endl;

    //circular_buffer
    circular_buffer<int>::iterator it;

    for (it = circularBuffer.begin(); it != circularBuffer.end(); it++)
    {
        std::cout << "Iterator: " << *it << std::endl;
    }


    return 0;
}