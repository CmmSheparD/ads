#include "searches.hh"

namespace searches {

int binarySearch(const std::vector<int>& vec, int value)
{
    if (vec.empty())
        return -1;
    // left - the lowest index of a slice to work on
    // right - the biggest index of a slice plus 1
    size_t left = 0, right = vec.size();
    size_t mid;
    while (left < right) {
        mid = (left + right) / 2;
        if (value < vec[mid])
            right = mid;
        else if (value > vec[mid])
            left = mid + 1;
        else
            break;
    }
    return left < right ? mid : -1;
}

}   // namespace searches
