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

template<class RandIt, class Compare>
void selectionSort(RandIt first, RandIt last, Compare cmp)
{
    if (first >= last)
        throw std::range_error("First iterator equals or is behind the last");
    RandIt shuttle;
    RandIt most_extreme;
    for (RandIt unsorted = first; unsorted != last; ++unsorted) {
        most_extreme = unsorted;
        for (shuttle = unsorted; shuttle != last; ++shuttle)
            if (cmp(*shuttle, *most_extreme)) most_extreme = shuttle;

        if (most_extreme != unsorted) std::swap(*most_extreme, *unsorted);
    }
}

template<class RandIt>
void selectionSort(RandIt first, RandIt last)
{
    selectionSort(first, last, std::less<typename RandIt::value_type>());
}

#endif	// SORTS_HH
