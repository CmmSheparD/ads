#include "../src/sorts.hh"

#include <iostream>
#include <random>
#include <vector>

#include <gtest/gtest.h>


TEST(SortEmpty, QuickSort)
{
    std::vector<int> v;
    ASSERT_NO_THROW(sorts::quickSort(v));
}

TEST(SortEmpty, BubbleSort)
{
    std::vector<int> v;
    ASSERT_NO_THROW(sorts::bubbleSort(v));
}

TEST(SortEmpty, Bogosort)
{
    std::vector<int> v;
    ASSERT_NO_THROW(sorts::bogoSort(v));
}

TEST(SortEmpty, CountingSort)
{
    std::vector<char> v;
    ASSERT_NO_THROW(sorts::countingSort(v));
}


TEST(QuickSort, InvalidArguments)
{
    std::vector<int> v;
    v.push_back(0);
    // let the high be too big
    ASSERT_THROW(sorts::quickSort(v, 0, 1), std::out_of_range);
    // let low be lesser than high
    ASSERT_THROW(sorts::quickSort(v, 1, 0), std::invalid_argument);
}


class SortTest : public ::testing::Test {
public:
    void SetUp()
    {
        std::default_random_engine rng;
        for (size_t i = 0; i < 100; ++i)
            vector_.push_back(rng());
    }
protected:
    std::vector<int> vector_;
};

TEST_F(SortTest, BubbleSort)
{
    ASSERT_NO_THROW(sorts::bubbleSort(vector_));
    for (size_t i = 1; i < vector_.size(); ++i) {
        ASSERT_TRUE(vector_[i - 1] <= vector_[i]);
    }

    ASSERT_NO_THROW(sorts::bubbleSort(vector_));
    for (size_t i = 1; i < vector_.size(); ++i) {
        ASSERT_TRUE(vector_[i - 1] <= vector_[i]);
    }
}

TEST_F(SortTest, QuickSort)
{
    ASSERT_NO_THROW(sorts::quickSort(vector_));
    for (size_t i = 1; i < vector_.size(); ++i) {
        ASSERT_TRUE(vector_[i - 1] <= vector_[i]);
    }

    ASSERT_NO_THROW(sorts::quickSort(vector_));
    for (size_t i = 1; i < vector_.size(); ++i) {
        ASSERT_TRUE(vector_[i - 1] <= vector_[i]);
    }
}


TEST(BogoSort, SortTest)
{
    std::vector<int> v = {10, 5, 4, 7, 0, 1, 3, 8, 9, 9};
    ASSERT_NO_THROW(sorts::bogoSort(v));
    for (size_t i = 1; i < v.size(); ++i) {
        ASSERT_TRUE(v[i - 1] <= v[i]);
    }
}


class CountingSortTest : public ::testing::Test {
public:
    void SetUp()
    {
        std::default_random_engine rng;
        for (size_t i = 0; i < 1000; ++i)
            vector_.push_back(rng() % 256);
    }
protected:
    std::vector<char> vector_;
};

TEST_F(CountingSortTest, SortTest)
{
    ASSERT_NO_THROW(sorts::countingSort(vector_));
    for (size_t i = 1; i < vector_.size(); ++i) {
        ASSERT_TRUE(vector_[i - 1] <= vector_[i]);
    }

    ASSERT_NO_THROW(sorts::countingSort(vector_));
    for (size_t i = 1; i < vector_.size(); ++i) {
        ASSERT_TRUE(vector_[i - 1] <= vector_[i]);
    }
}
