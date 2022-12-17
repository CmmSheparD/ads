#include "../src/stack.hh"

#include <stdexcept>

#include <gtest/gtest.h>

using namespace data_structs;

TEST(Base, StackCreationTest)
{
    Stack<int> s;
    ASSERT_EQ(s.size(), 0);
    ASSERT_TRUE(s.is_empty());
    ASSERT_THROW(s.peek(), std::logic_error);
    ASSERT_NO_THROW(s.pop());
}

TEST(Base, StackEmptyInsert)
{
    Stack<int> s;
    ASSERT_NO_THROW(s.push(0));
    ASSERT_EQ(s.size(), 1);
    ASSERT_FALSE(s.is_empty());
    ASSERT_EQ(s.peek(), 0);
}


class StackTest : public ::testing::Test {
public:
    void SetUp()
    {
        for (int i = 0; i < 10; ++i)
            s_.push(i);
    }
protected:
    Stack<int> s_;
};

TEST_F(StackTest, PushTest)
{
    ASSERT_NO_THROW(s_.push(10));
    ASSERT_EQ(s_.size(), 11);
    ASSERT_FALSE(s_.is_empty());
    ASSERT_EQ(s_.peek(), 10);
}

TEST_F(StackTest, PopTest)
{
    for (int i = 9; i >= 0; --i) {
        ASSERT_EQ(s_.peek(), i);
        ASSERT_NO_THROW(s_.pop());
    }
    ASSERT_TRUE(s_.is_empty());
}
