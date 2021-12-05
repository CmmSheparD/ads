#include "../src/stack.hh"

#include <stdexcept>

#include <gtest/gtest.h>

using namespace data_structs;

TEST(Base, StackCreationTest)
{
    Stack<int> q;
    ASSERT_EQ(q.size(), 0);
    ASSERT_TRUE(q.is_empty());
    ASSERT_THROW(q.top(), std::logic_error);
    ASSERT_NO_THROW(q.pop());
}

TEST(Base, StackEmptyInsert)
{
    Stack<int> q;
    ASSERT_NO_THROW(q.push(0));
    ASSERT_EQ(q.size(), 1);
    ASSERT_FALSE(q.is_empty());
    ASSERT_EQ(q.top(), 0);
}


class StackTest : public ::testing::Test {
public:
    void SetUp()
    {
        for (int i = 0; i < 10; ++i)
            q_.push(i);
    }
protected:
    Stack<int> q_;
};

TEST_F(StackTest, PushTest)
{
    ASSERT_NO_THROW(q_.push(10));
    ASSERT_EQ(q_.size(), 11);
    ASSERT_FALSE(q_.is_empty());
    ASSERT_EQ(q_.top(), 10);
}

TEST_F(StackTest, PopTest)
{
    for (int i = 9; i >= 0; --i) {
        ASSERT_EQ(q_.top(), i);
        ASSERT_NO_THROW(q_.pop());
    }
}
