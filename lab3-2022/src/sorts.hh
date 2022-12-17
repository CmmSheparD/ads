#pragma once
#ifndef SORTS_HH
#define SORTS_HH

#include <functional>
#include <iostream>
#include <stdexcept>

template<class RandIt, class Compare>
void insertionSort(RandIt first, RandIt last, Compare cmp)
{
    if (first >= last)
        throw std::range_error("First iterator equals or is behind the last");
    RandIt shuttle, prev;
    for (RandIt cur = first + 1; cur < last; ++cur) {
        for (shuttle = cur, prev = cur - 1;
             shuttle != first && cmp(*shuttle, *(shuttle - 1));
             --shuttle, --prev)

            std::swap(*shuttle, *(shuttle - 1));
    }
}

template<class RandIt>
void insertionSort(RandIt first, RandIt last)
{
    insertionSort(first, last, std::less<typename RandIt::value_type>());
}

#endif	// SORTS_HH
