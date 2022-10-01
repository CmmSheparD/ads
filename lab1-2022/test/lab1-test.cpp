#include "../src/list.hh"

#include <iostream>
#include <stdexcept>

#include <gtest/gtest.h>

using list::LinkedList;
using namespace std;

void printList(LinkedList<int> &list)
{
    cout << "{";
    for (size_t i = 0; i < list.get_size(); ++i) {
        if (i != list.get_size() - 1)
            cout << ", ";
        cout << list.at(i);
    }
    cout << "}" << endl;
}

TEST(Base, Creation)
{
    LinkedList<int> list;
    printList(list);
    ASSERT_EQ(list.get_size(), 0);
    ASSERT_TRUE(list.is_empty());
}

TEST(Base, GetSize)
{
    LinkedList<int> list;
    ASSERT_EQ(list.get_size(), 0);
}

TEST(Base, IsEmpty)
{
    LinkedList<int> list;
    ASSERT_TRUE(list.is_empty());
    list.push_back(0);
    ASSERT_FALSE(list.is_empty());
}

TEST(Base, CopyConstruct)
{
    LinkedList<int> list0;
    list0.push_back(0);
    list0.push_back(1);
    list0.push_back(2);
    list0.push_back(3);
    list0.push_back(4);
    LinkedList<int> list1 = list0;
    ASSERT_FALSE(list1.is_empty());
    ASSERT_EQ(list1.get_size(), list0.get_size());
    for (size_t i = 0; i < list0.get_size(); ++i)
        ASSERT_EQ(list0.at(i), list1.at(i));
}


class DLinkedListEmptyTest : public ::testing::Test {
protected:
    LinkedList<int> list_;
};

TEST_F(DLinkedListEmptyTest, PushBack)
{
    list_.push_back(0);
    printList(list_);
    ASSERT_EQ(list_.get_size(), 1);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListEmptyTest, PushFront)
{
    list_.push_front(0);
    printList(list_);
    ASSERT_EQ(list_.get_size(), 1);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListEmptyTest, PopBack)
{
    list_.pop_back();
    printList(list_);
    ASSERT_EQ(list_.get_size(), 0);
    ASSERT_TRUE(list_.is_empty());
}

TEST_F(DLinkedListEmptyTest, PopFront)
{
    list_.pop_front();
    printList(list_);
    ASSERT_EQ(list_.get_size(), 0);
    ASSERT_TRUE(list_.is_empty());
}

TEST_F(DLinkedListEmptyTest, Insert)
{
    ASSERT_THROW(list_.insert(0, 1), std::out_of_range);
    printList(list_);
    list_.insert(0, 0);
    ASSERT_EQ(list_.get_size(), 1);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListEmptyTest, Remove)
{
    ASSERT_THROW(list_.remove(0), std::out_of_range);
    ASSERT_THROW(list_.remove(2), std::out_of_range);
    printList(list_);
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
    printList(list_);
}

TEST_F(DLinkedListEmptyTest, Swap)
{
    ASSERT_THROW(list_.swap(0, 1), std::out_of_range);
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

    LinkedList<int> list_;
};

TEST_F(DLinkedListTest, PushBack)
{
    size_t s = list_.get_size() + 1;
    list_.push_back(0);
    printList(list_);
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListTest, PushFront)
{
    size_t s = list_.get_size() + 1;
    list_.push_front(0);
    printList(list_);
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListTest, PopBack)
{
    size_t s = list_.get_size() - 1;
    list_.pop_back();
    printList(list_);
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListTest, PopFront)
{
    size_t s = list_.get_size() - 1;
    list_.pop_front();
    printList(list_);
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListTest, Insert)
{
    size_t s = list_.get_size() + 1;
    ASSERT_THROW(list_.insert(0, s), std::out_of_range);
    printList(list_);

    list_.insert(5, 0);
    printList(list_);
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());

    list_.insert(5, s++);
    printList(list_);
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());

    list_.insert(10, s/2);
    std::cout << "s/2: " << s/2 << std::endl;
    printList(list_);
    ASSERT_EQ(list_.get_size(), ++s);
    ASSERT_FALSE(list_.is_empty());
}

TEST_F(DLinkedListTest, Remove)
{
    size_t s = list_.get_size();
    ASSERT_THROW(list_.remove(s), std::out_of_range);
    printList(list_);
    ASSERT_EQ(list_.get_size(), s);
    ASSERT_FALSE(list_.is_empty());

    list_.remove(s/2);
    printList(list_);
    ASSERT_EQ(list_.get_size(), --s);
    ASSERT_FALSE(list_.is_empty());

    list_.remove(0);
    printList(list_);
    ASSERT_EQ(list_.get_size(), --s);
    ASSERT_FALSE(list_.is_empty());

    list_.remove(--s);
    printList(list_);
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
    printList(list_);
}

TEST_F(DLinkedListTest, Clear)
{
    list_.clear();
    ASSERT_EQ(list_.get_size(), 0);
    ASSERT_TRUE(list_.is_empty());
    printList(list_);
}

TEST_F(DLinkedListTest, Swap)
{
    list_.swap(0, 1);
    ASSERT_EQ(list_.at(0), 1);
    ASSERT_EQ(list_.at(1), 0);
}

TEST_F(DLinkedListTest, SwaOutOfRange)
{
    ASSERT_THROW(list_.swap(0, 4), std::out_of_range);
}