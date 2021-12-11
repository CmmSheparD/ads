#include "searches.hh"

namespace searches {

int binarySearch(int array[], size_t n, int value)
{
    if (array == nullptr || n == 0)
        return -1;
    // left - the lowest index of a slice to work on
    // right - the biggest index of a slice plus 1
    size_t left = 0, right = n;
    size_t mid;
    while (left < right) {
        mid = (left + right) / 2;
        if (value < array[mid])
            right = mid;
        else if (value > array[mid])
            left = mid + 1;
        else
            break;
    }
    return left < right ? mid : -1;
}

}   // namespace searches
