#include "sorts.hh"

#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>
using std::cout;
using std::endl;

namespace sorts {

void quickSort(std::vector<int>& vector, size_t low, long long high)
{
    if (vector.empty())
        return;
    else if (high == -1)
        high = vector.size() - 1;
    else if (high > vector.size() - 1)
        throw std::out_of_range("High is out of range.");
    if (low > high)
        throw std::invalid_argument("Low is greater than high.");
    if (high - low < 1)
        return;

    int pivot_index;
    int pivot_val = vector[(low + high) / 2];
    size_t i = low;
    size_t j = high;
    while (true) {
        while (vector[i] < pivot_val)
            ++i;
        while (vector[j] > pivot_val)
            --j;
        if (i >= j) {
            pivot_index = j;
            break;
        }
        int tmp = vector[i];
        vector[i] = vector[j];
        vector[j] = tmp;
        ++i;
        --j;
    }
    quickSort(vector, low, pivot_index);
    quickSort(vector, pivot_index + 1, high);
}


void bubbleSort(std::vector<int>& vector)
{
    int buf;
    bool sorted;
    do {
        sorted = true;
        for (size_t i = 1; i < vector.size(); ++i) {
            if (vector[i - 1] > vector[i]) {
                buf = vector[i - 1];
                vector[i - 1] = vector[i];
                vector[i] = buf;
                sorted = false;
            }
        }
    } while (!sorted);
}


bool isSorted(std::vector<int>& vector)
{
    for (size_t i = 0; i < vector.size() - 1; ++i) {
        if (vector[i] > vector[i + 1])
            return false;
    }
    return true;
}

void bogoSort(std::vector<int>& vector)
{
    if (vector.size() < 2)
        return;

    std::default_random_engine rng;
    while (!isSorted(vector)) {
        for (size_t i = 0; i < vector.size(); ++i) {
            size_t rand_index = rng() % vector.size();
            int tmp = vector[i];
            vector[i] = vector[rand_index];
            vector[rand_index] = tmp;
        }
    }
}

void countingSort(std::vector<char>& vector)
{
    if (vector.size() < 2)
        return;

    // in case char is a signed type
    int max = 0, min = 0;
    for (char c : vector) {
        if ((int)c > max)
            max = c;
        if ((int)c < min)
            min = c;
    }
    std::vector<size_t> counters(max - min + 1, (size_t)0);
    for (char c : vector) {
        ++counters[c - min];
    }
    for (int i = min, j = 0; i < max + 1; ++i) {
        while (counters[i - min]-- > 0)
            vector[j++] = (char)i;
    }
}

}   //namespace sorts
