#include "../src/dlist.hh"

#include <iostream>
#include <stdexcept>

#include <gtest/gtest.h>

using dlist::DLinkedList;


TEST(Base, Creation)
{
    DLinkedList list;
    std::cout << list << std::endl;
    ASSERT_EQ(list.get_size(), 0);
    ASSERT_TRUE(list.is_empty());
}

TEST(Base, GetSize)
{
    DLinkedList list;
    ASSERT_EQ(list.get_size(), 0);
}

TEST(Base, IsEmpty)
{
    DLinkedList list;
    ASSERT_TRUE(list.is_empty());
    list.push_back(0);
    ASSERT_FALSE(list.is_empty());
}


class DLinkedListEmptyTest : public ::testing::Test {
protected:
    DLinkedList list_;
};

TEST_F(DLinkedListEmptyTest, PushBack)
{
    list_.push_back(0);
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), 1);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListEmptyTest, PushFront)
{
    list_.push_front(0);
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), 1);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListEmptyTest, PopBack)
{
    list_.pop_back();
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), 0);
    ASSERT_TRUE(list_.is_empty());
}

TEST_F(DLinkedListEmptyTest, PopFront)
{
    list_.pop_front();
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), 0);
    ASSERT_TRUE(list_.is_empty());
}

TEST_F(DLinkedListEmptyTest, Insert)
{
    ASSERT_THROW(list_.insert(0, 1), std::out_of_range);
    std::cout << list_ << std::endl;
    list_.insert(0, 0);
    ASSERT_EQ(list_.get_size(), 1);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListEmptyTest, Remove)
{
    ASSERT_THROW(list_.remove(0), std::out_of_range);
    ASSERT_THROW(list_.remove(2), std::out_of_range);
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), 0);
    ASSERT_TRUE(list_.is_empty());
}

TEST_F(DLinkedListEmptyTest, At)
{
    ASSERT_THROW(list_.at(0), std::out_of_range);
    ASSERT_THROW(list_.at(2), std::out_of_range);
}

TEST_F(DLinkedListEmptyTest, Set)
{
    ASSERT_THROW(list_.set(0, 1), std::out_of_range);
    ASSERT_THROW(list_.set(2, 1), std::out_of_range);
}

TEST_F(DLinkedListEmptyTest, Clear)
{
    list_.clear();
    ASSERT_EQ(list_.get_size(), 0);
    ASSERT_TRUE(list_.is_empty());
    std::cout << list_ << std::endl;
}


TEST_F(DLinkedListEmptyTest, InsertList)
{
    DLinkedList list;
    ASSERT_THROW(list_.insert(list, 1), std::out_of_range);
    list_.insert(list, 0);
    std::cout << "list_ " << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), 0);
    ASSERT_TRUE(list_.is_empty());

    list.push_back(0);
    std::cout << "list " << list << std::endl;
    std::cout << "list_ "<< list_ << std::endl;
    list_.insert(list, 0);
    std::cout << "list " << list << std::endl;
    std::cout << "list_ "<< list_ << std::endl;
    ASSERT_EQ(list_.get_size(), 1);
    ASSERT_FALSE(list_.is_empty());
    ASSERT_EQ(list_.at(0), 0);

    ASSERT_EQ(list.get_size(), 0);
    ASSERT_TRUE(list.is_empty());

    list_.clear();
    list.push_back(0);
    list.push_back(1);
    list.push_back(2);
    std::cout << "list " << list << std::endl;
    std::cout << "list_ "<< list_ << std::endl;
    list_.insert(list, 0);
    std::cout << "list " << list << std::endl;
    std::cout << "list_ "<< list_ << std::endl;
    ASSERT_EQ(list_.get_size(), 3);
    ASSERT_FALSE(list_.is_empty());

    ASSERT_EQ(list.get_size(), 0);
    ASSERT_TRUE(list.is_empty());
}


class DLinkedListTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        list_.push_back(0);
        list_.push_back(1);
        list_.push_back(2);
        list_.push_back(3);
    }

    DLinkedList list_;
};

TEST_F(DLinkedListTest, PushBack)
{
    size_t s = list_.get_size() + 1;
    list_.push_back(0);
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListTest, PushFront)
{
    size_t s = list_.get_size() + 1;
    list_.push_front(0);
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListTest, PopBack)
{
    size_t s = list_.get_size() - 1;
    list_.pop_back();
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListTest, PopFront)
{
    size_t s = list_.get_size() - 1;
    list_.pop_front();
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListTest, Insert)
{
    size_t s = list_.get_size() + 1;
    ASSERT_THROW(list_.insert(0, s), std::out_of_range);
    std::cout << list_ << std::endl;

    list_.insert(5, 0);
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());

    list_.insert(5, s++);
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());

    list_.insert(10, s/2);
    std::cout << "s/2: " << s/2 << std::endl;
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), ++s);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListTest, Remove)
{
    size_t s = list_.get_size();
    ASSERT_THROW(list_.remove(s), std::out_of_range);
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());

    list_.remove(s/2);
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), --s);
    ASSERT_FALSE(list_.is_empty());

    list_.remove(0);
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), --s);
    ASSERT_FALSE(list_.is_empty());

    list_.remove(--s);
    std::cout << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListTest, At)
{
    ASSERT_THROW(list_.at(10), std::out_of_range);
    for (size_t i = 0; i < list_.get_size(); ++i)
        ASSERT_EQ(list_.at(i), i);
}

TEST_F(DLinkedListTest, Set)
{
    size_t s = list_.get_size();
    ASSERT_THROW(list_.set(s, 1), std::out_of_range);
    for (size_t i = 0; i < s; ++i) {
        list_.set(i, s);
        ASSERT_EQ(list_.at(i), s);
    }
    std::cout << list_ << std::endl;
}

TEST_F(DLinkedListTest, Clear)
{
    list_.clear();
    ASSERT_EQ(list_.get_size(), 0);
    ASSERT_TRUE(list_.is_empty());
    std::cout << list_ << std::endl;
}

TEST_F(DLinkedListTest, InsertEmptyList)
{
    DLinkedList list;
    size_t s = list_.get_size();
    list_.insert(list, 0);
    std::cout << "list " << list << std::endl;
    std::cout << "list_ " << list_ << std::endl;
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListTest, InsertListAtHead)
{
    DLinkedList list;
    size_t s = list_.get_size();
    list.push_back(10);
    std::cout << "list " << list << std::endl;
    std::cout << "list_ "<< list_ << std::endl;
    list_.insert(list, 0);
    std::cout << "list " << list << std::endl;
    std::cout << "list_ "<< list_ << std::endl;
    ASSERT_EQ(list_.get_size(), ++s);
    ASSERT_FALSE(list_.is_empty());

    ASSERT_EQ(list.get_size(), 0);
    ASSERT_TRUE(list.is_empty());
}

TEST_F(DLinkedListTest, InsertListInMiddle)
{
    DLinkedList list;
    size_t s = list_.get_size();
    list.push_back(0);
    list.push_back(1);
    list.push_back(2);
    std::cout << "list " << list << std::endl;
    std::cout << "list_ "<< list_ << std::endl;
    list_.insert(list, s/2);
    s += 3;
    std::cout << "list " << list << std::endl;
    std::cout << "list_ "<< list_ << std::endl;
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());

    ASSERT_EQ(list.get_size(), 0);
    ASSERT_TRUE(list.is_empty());
}

TEST_F(DLinkedListTest, InsertListAtTail)
{
    DLinkedList list;
    size_t s = list_.get_size();
    list.push_front(0);
    list.push_front(1);
    list.push_front(2);
    std::cout << "list " << list << std::endl;
    std::cout << "list_ "<< list_ << std::endl;
    list_.insert(list, s);
    s += 3;
    std::cout << "list " << list << std::endl;
    std::cout << "list_ "<< list_ << std::endl;
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());

    ASSERT_EQ(list.get_size(), 0);
    ASSERT_TRUE(list.is_empty());
}
