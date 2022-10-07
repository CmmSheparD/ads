#include "../src/searches.hh"

#include <gtest/gtest.h>

using searches::binarySearch;


class BinarySearchTest : public ::testing::Test {
public:
    void SetUp() override
    {
        array_ = new int[11];
        n_ = 11;
        array_[0] =  {-1};
        array_[1] =  {1};
        array_[2] =  {2};
        array_[3] =  {3};
        array_[4] =  {5};
        array_[5] =  {7};
        array_[6] =  {8};
        array_[7] =  {9};
        array_[8] =  {10};
        array_[9] =  {26};
        array_[10] =  {100};
    }

    void TearDown() override
    {
        delete[] array_;
    }
protected:
    int *array_;
    size_t n_;
};


TEST_F(BinarySearchTest, SearchEveryOne)
{
    for (size_t i = 0; i < n_; ++i) {
        ASSERT_EQ(array_[i], array_[binarySearch(array_, n_, array_[i])]);
    }
}

TEST_F(BinarySearchTest, SearchNonExistent)
{
    ASSERT_EQ(binarySearch(array_, n_, 0), -1);
}

TEST_F(BinarySearchTest, SearchNonExistentLesser)
{
    ASSERT_EQ(binarySearch(array_, n_, -10), -1);
}

TEST_F(BinarySearchTest, SearchNonExistentBigger)
{
    ASSERT_EQ(binarySearch(array_, n_, 120), -1);
}
