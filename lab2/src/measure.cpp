#include <iostream>
#include <chrono>
#include <random>
#include <vector>

#include "sorts.hh"

std::vector<int> generateRandomVector(size_t size)
{
    std::vector<int> vector;
    std::default_random_engine rng;
    rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
    for (size_t i = 0; i < size; ++i)
        vector.push_back(rng());
    return vector;
}


int main()
{
    size_t sizes[] = {10, 100, 1000, 10000, 100000};
    std::chrono::steady_clock timer;
    std::cout << "Average quick sorting time on vector of:" << std::endl;
    for (size_t s : sizes) {
        std::cout << s << " elements: ";
        decltype(timer.now()) beg, fin;
        auto dur = beg - beg;
        for (int i = 0; i < 10; ++i) {
            std::vector<int> v = generateRandomVector(s);
            beg = timer.now();
            sorts::quickSort(v);
            fin = timer.now();
            dur += fin - beg;
        }
        dur /= 10;
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(dur).count() << " us" << std::endl;
    }
    std::cout << "Average bubble sorting time on vector of:" << std::endl;
    for (size_t s : sizes) {
        std::cout << s << " elements: ";
        decltype(timer.now()) beg, fin;
        auto dur = beg - beg;
        for (int i = 0; i < 10; ++i) {
            std::vector<int> v = generateRandomVector(s);
            beg = timer.now();
            sorts::bubbleSort(v);
            fin = timer.now();
            dur += fin - beg;
        }
        dur /= 10;
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(dur).count() << " us" << std::endl;
    }
    return 0;
}
