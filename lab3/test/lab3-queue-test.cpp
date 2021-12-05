#include "../src/queue.hh"

#include <stdexcept>

#include <gtest/gtest.h>

using namespace data_structs;

TEST(Base, QueueCreationTest)
{
    Queue<int> q;
    ASSERT_EQ(q.size(), 0);
    ASSERT_TRUE(q.is_empty());
    ASSERT_THROW(q.front(), std::logic_error);
    ASSERT_NO_THROW(q.pop());
}

TEST(Base, QueueEmptyInsert)
{
    Queue<int> q;
    ASSERT_NO_THROW(q.push(0));
    ASSERT_EQ(q.size(), 1);
    ASSERT_FALSE(q.is_empty());
    ASSERT_EQ(q.front(), 0);
}


class QueueTest : public ::testing::Test {
public:
    void SetUp()
    {
        for (int i = 0; i < 10; ++i)
            q_.push(i);
    }
protected:
    Queue<int> q_;
};

TEST_F(QueueTest, PushTest)
{
    ASSERT_NO_THROW(q_.push(10));
    ASSERT_EQ(q_.size(), 11);
    ASSERT_FALSE(q_.is_empty());
    ASSERT_EQ(q_.front(), 0);
}

TEST_F(QueueTest, PopTest)
{
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(q_.front(), i);
        ASSERT_NO_THROW(q_.pop());
    }
}
