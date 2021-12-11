#include "../src/sorts.hh"

#include <iostream>
#include <random>

#include <gtest/gtest.h>


TEST(SortEmpty, QuickSort)
{
    int *a = nullptr;
    ASSERT_NO_THROW(sorts::quickSort(a, 0));
}

TEST(SortEmpty, BubbleSort)
{
    int *a = nullptr;
    ASSERT_NO_THROW(sorts::bubbleSort(a, 0));
}

TEST(SortEmpty, Bogosort)
{
    int *a = nullptr;
    ASSERT_NO_THROW(sorts::bogoSort(a, 0));
}

TEST(SortEmpty, CountingSort)
{
    char *a = nullptr;
    ASSERT_NO_THROW(sorts::countingSort(a, 0));
}


TEST(QuickSort, InvalidArguments)
{
    int a[] = {0};
    // let the high be too big
    ASSERT_THROW(sorts::quickSort(a, 1, 0, 1), std::out_of_range);
    // let low be lesser than high
    ASSERT_THROW(sorts::quickSort(a, 1, 1, 0), std::invalid_argument);
}


class SortTest : public ::testing::Test {
public:
    void SetUp() override
    {
        array_ = new int[100];
        std::default_random_engine rng;
        for (size_t i = 0; i < 100; ++i)
            array_[i] = rng();
    }
    void TearDown() override
    {
        delete[] array_;
    }
protected:
    int *array_;
    size_t n_;
};

TEST_F(SortTest, BubbleSort)
{
    ASSERT_NO_THROW(sorts::bubbleSort(array_, n_));
    for (size_t i = 1; i < n_; ++i)
        ASSERT_TRUE(array_[i - 1] <= array_[i]);

    ASSERT_NO_THROW(sorts::bubbleSort(array_, n_));
    for (size_t i = 1; i < n_; ++i)
        ASSERT_TRUE(array_[i - 1] <= array_[i]);
}

TEST_F(SortTest, QuickSort)
{
    ASSERT_NO_THROW(sorts::quickSort(array_, n_));
    for (size_t i = 1; i < n_; ++i)
        ASSERT_TRUE(array_[i - 1] <= array_[i]);

    ASSERT_NO_THROW(sorts::quickSort(array_, n_));
    for (size_t i = 1; i < n_; ++i)
        ASSERT_TRUE(array_[i - 1] <= array_[i]);
}


TEST(BogoSort, SortTest)
{
    int array[] = {10, 5, 4, 7, 0, 1, 3, 8, 9, 9};
    ASSERT_NO_THROW(sorts::bogoSort(array, 10));
    for (size_t i = 1; i < 10; ++i)
        ASSERT_TRUE(array[i - 1] <= array[i]);
}


class CountingSortTest : public ::testing::Test {
public:
    void SetUp()
    {
        array_ = new char[1000];
        std::default_random_engine rng;
        for (size_t i = 0; i < 1000; ++i)
            array_[i] = rng() % 256;
        n_ = 1000;
    }
    void TearDown() override
    {
        delete[] array_;
    }
protected:
    char *array_;
    size_t n_;
};

TEST_F(CountingSortTest, SortTest)
{
    ASSERT_NO_THROW(sorts::countingSort(array_, n_));
    for (size_t i = 1; i < n_; ++i) {
        ASSERT_TRUE(array_[i - 1] <= array_[i]);
    }

    ASSERT_NO_THROW(sorts::countingSort(array_, n_));
    for (size_t i = 1; i < n_; ++i) {
        ASSERT_TRUE(array_[i - 1] <= array_[i]);
    }
}
