#pragma once
#ifndef SORTS_HH
#define SORTS_HH


#include <cstddef>

namespace sorts {

void quickSort(int array[], size_t n, size_t low = 0, long long high = -1);

void bubbleSort(int array[], size_t n);

void bogoSort(int array[], size_t n);

void countingSort(char array[], size_t n);

}   // namespace sorts

#endif  // SORTS_HH
