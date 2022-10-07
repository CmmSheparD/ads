#include "sorts.hh"

#include <iostream>
#include <random>
#include <stdexcept>
using std::cout;
using std::endl;

namespace sorts {

void quickSort(int array[], size_t n, size_t low, long long high)
{
    if (n == 0)
        return;
    else if (high == -1)
        high = n - 1;
    else if ((size_t)high > n - 1)
        throw std::out_of_range("High is out of range.");
    if ((long long)low > high)
        throw std::invalid_argument("Low is greater than high.");
    if (high - low < 1)
        return;

    int pivot_index;
    int pivot_val = array[(low + high) / 2];
    size_t i = low;
    size_t j = high;
    while (true) {
        while (array[i] < pivot_val)
            ++i;
        while (array[j] > pivot_val)
            --j;
        if (i >= j) {
            pivot_index = j;
            break;
        }
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
        ++i;
        --j;
    }
    quickSort(array, n, low, pivot_index);
    quickSort(array, n, pivot_index + 1, high);
}


void bubbleSort(int array[], size_t n)
{
    int buf;
    bool sorted;
    do {
        sorted = true;
        for (size_t i = 1; i < n; ++i) {
            if (array[i - 1] > array[i]) {
                buf = array[i - 1];
                array[i - 1] = array[i];
                array[i] = buf;
                sorted = false;
            }
        }
    } while (!sorted);
}


bool isSorted(int array[], size_t n)
{
    for (size_t i = 0; i < n - 1; ++i) {
        if (array[i] > array[i + 1])
            return false;
    }
    return true;
}

void bogoSort(int array[], size_t n)
{
    if (n < 2)
        return;

    std::default_random_engine rng;
    while (!isSorted(array, n)) {
        for (size_t i = 0; i < n; ++i) {
            size_t rand_index = rng() % n;
            int tmp = array[i];
            array[i] = array[rand_index];
            array[rand_index] = tmp;
        }
    }
}

void countingSort(char array[], size_t n)
{
    if (n < 2)
        return;

    // in case char is a signed type
    int max = array[0], min = array[0];
    for (size_t i = 1; i < n; ++i) {
        if ((int)array[i] > max)
            max = array[i];
        if ((int)array[i] < min)
            min = array[i];
    }
    int *counters = new int[max - min + 1];
    for (int i = min; i < max + 1; ++i)
        counters[i - min] = 0;
    for (size_t i = 0; i < n; ++i) {
        ++counters[array[i] - min];
    }
    for (int i = min, j = 0; i < max + 1; ++i) {
        while (counters[i - min]-- > 0)
            array[j++] = (char)i;
    }
}

}   //namespace sorts
