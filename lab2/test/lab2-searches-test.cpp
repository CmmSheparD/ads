#include "../src/searches.hh"

#include <vector>

#include <gtest/gtest.h>

using searches::binarySearch;


class BinarySearchTest : public ::testing::Test {
public:
    void SetUp() override
    {
        vector_ = {-1, 1, 2, 3, 5, 7, 8, 9, 10, 26, 100};
    }
protected:
    std::vector<int> vector_;
};


TEST_F(BinarySearchTest, SearchEveryOne)
{
    for (size_t i = 0; i < vector_.size(); ++i) {
        ASSERT_EQ(vector_[i], vector_[binarySearch(vector_, vector_[i])]);
    }
}

TEST_F(BinarySearchTest, SearchNonExistent)
{
    ASSERT_EQ(binarySearch(vector_, 0), -1);
}

TEST_F(BinarySearchTest, SearchNonExistentLesser)
{
    ASSERT_EQ(binarySearch(vector_, -10), -1);
}

TEST_F(BinarySearchTest, SearchNonExistentBigger)
{
    ASSERT_EQ(binarySearch(vector_, 120), -1);
}
