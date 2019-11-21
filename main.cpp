#include <iostream>
#include "src/circular_buffer.hpp"
#include "src/static_circular_buffer.hpp"

int main()
{
    circular_buffer<int> circularBuffer(3);
    std::cout << "Empty: " << (circularBuffer.empty() ? "true" : "false") << std::endl;

    circularBuffer.push_back(1);
    circularBuffer.push_back(2);
    circularBuffer.push_back(3);

    std::cout << circularBuffer.front() << std::endl;
    std::cout << circularBuffer.back() << std::endl;

    circularBuffer.resize(2);

    std::cout << circularBuffer.front() << std::endl;
    std::cout << circularBuffer.back() << std::endl;

    return 0;
}