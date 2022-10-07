#include <iostream>
#include <chrono>
#include <random>

#include "sorts.hh"

int *generateRandomArray(size_t size)
{
    int *array = new int[size];
    std::default_random_engine rng;
    rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
    for (size_t i = 0; i < size; ++i)
        array[i] = rng();
    return array;
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
            int *array = generateRandomArray(s);
            beg = timer.now();
            sorts::quickSort(array, s);
            fin = timer.now();
            dur += fin - beg;
            delete[] array;
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
            int *array = generateRandomArray(s);
            beg = timer.now();
            sorts::bubbleSort(array, s);
            fin = timer.now();
            dur += fin - beg;
            delete[] array;
        }
        dur /= 10;
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(dur).count() << " us" << std::endl;
    }
    return 0;
}
