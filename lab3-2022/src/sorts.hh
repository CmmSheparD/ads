#pragma once
#ifndef SORTS_HH
#define SORTS_HH

#include <functional>
#include <stdexcept>
#include <vector>

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

template<class BidirIt, class Compare>
void bubbleSort(BidirIt first, BidirIt last, Compare cmp)
{
    if (first == last)
        throw std::range_error("First iterator equals the last");
    BidirIt cur, prev;
    bool sorted;
    do {
        sorted = true;
        cur = prev = first;
        for (++cur; cur != last; ++cur, ++prev) {
            if (cmp(*cur, *prev)) {
                std::swap(*cur, *prev);
                sorted = false;
            }
        }
    } while (!sorted);
}

template<class BidirIt>
void bubbleSort(BidirIt first, BidirIt last)
{
    bubbleSort(first, last, std::less<typename BidirIt::value_type>());
}

template<class RandIt, class Compare>
void mergeSort(RandIt first, RandIt last, Compare cmp)
{
    if (first >= last)
        throw std::range_error("First iterator equals or is behind the last");
    const typename RandIt::difference_type len = last - first;
    if (len == 1) return;
    if (len == 2) {
        if (cmp(*(first + 1), *first)) std::swap(*(first + 1), *first);
        return;
    }
    RandIt mid = first + (len / 2 + len % 2);
    mergeSort(first, mid, cmp);
    mergeSort(mid, last, cmp);

    typename std::vector<typename RandIt::value_type> buffer;
    buffer.reserve(len);
    typename std::vector<typename RandIt::value_type>::iterator it = buffer.begin();
    RandIt lcandidate = first, rcandidate = mid;
    while (lcandidate != mid && rcandidate != last) {
        *it++ = cmp(*lcandidate, *rcandidate) ? *lcandidate++ : *rcandidate++;
    }
    if (lcandidate == mid) lcandidate = rcandidate;
    while (lcandidate != last) *it++ = *lcandidate++;
    lcandidate = first;
    it = buffer.begin();
    while (lcandidate != last) *lcandidate++ = *it++;
}

template<class RandIt>
void mergeSort(RandIt first, RandIt last)
{
    mergeSort(first, last, std::less<typename RandIt::value_type>());
}

template<class RandIt, class Compare>
void shellSort(RandIt first, RandIt last, Compare cmp)
{
    if (first >= last)
        throw std::range_error("First iterator equals or is behind the last");
    const typename RandIt::difference_type len = last - first;
    typename RandIt::difference_type gap = len;
    RandIt cur, pair;
    while (gap != 1) {
        gap /= 2;
        if (gap < 1) gap = 1;
        for (cur = first + gap; cur != last; ++cur) {
            for (pair = cur - gap; pair >= first; pair -= gap)
                if (cmp(*(pair + gap), *pair))
                    std::swap(*(pair + gap), *pair);
        }
    }
}

template<class RandIt>
void shellSort(RandIt first, RandIt last)
{
    shellSort(first, last, std::less<typename RandIt::value_type>());
}

template<class RandIt, class Compare>
void quickSort(RandIt first, RandIt last, Compare cmp)
{
    if (first >= last)
        throw std::range_error("First iterator equals or is behind the last");
    const typename RandIt::difference_type len = last - first;
    if (len == 1) return;
    if (len == 2) {
        if (cmp(*(first + 1), *first)) std::swap(*(first + 1), *first);
        return;
    }
    RandIt pivot = first + len / 2;
    typename RandIt::value_type pivot_val = *pivot;
    RandIt left = first, right = last - 1;
    while (true) {
        while (left < last && cmp(*left, pivot_val)) ++left;
        while (right > first && cmp(pivot_val, *right)) --right;
        if (left >= right) {
            pivot = first == right ? right + 1 : right;
            break;
        }
        std::swap(*left, *right);
        ++left;
        --right;
    }
    quickSort(first, pivot, cmp);
    quickSort(pivot, last, cmp);
}

template<class RandIt>
void quickSort(RandIt first, RandIt last)
{
    quickSort(first, last, std::less<typename RandIt::value_type>());
}

#endif	// SORTS_HH
