#pragma once
#ifndef SORTS_HH
#define SORTS_HH


#include <vector>

namespace sorts {

void quickSort(std::vector<int>& vector, size_t low = 0, long long high = -1);

void bubbleSort(std::vector<int>& vector);

void bogoSort(std::vector<int>& vector);

void countingSort(std::vector<char>& vector);

}   // namespace sorts

#endif  // SORTS_HH
