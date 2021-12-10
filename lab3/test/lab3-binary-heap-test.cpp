#include "../src/binary_heap.hh"

#include <cmath>
#include <iostream>

#include <gtest/gtest.h>

using namespace data_structs;
using std::cout;
using std::endl;


TEST(Base, CreationTest)
{
    BinaryHeap heap(BinaryHeap::kMinHeap);
    ASSERT_EQ(heap.count(), 0);
    ASSERT_TRUE(heap.is_empty());
}

TEST(Base, AddOneElement)
{
    BinaryHeap heap(BinaryHeap::kMinHeap);
    heap.insert(0);
    cout << heap << endl;
    ASSERT_EQ(heap.count(), 1);
    ASSERT_FALSE(heap.is_empty());
}

TEST(Base, AddFewElements)
{
    BinaryHeap heap(BinaryHeap::kMinHeap);
    heap.insert(0);
    cout << heap << endl;
    heap.insert(1);
    cout << heap << endl;
    heap.insert(2);
    cout << heap << endl;
    heap.insert(3);
    cout << heap << endl;
    heap.insert(4);
    cout << heap << endl;
    ASSERT_EQ(heap.count(), 5);
    ASSERT_FALSE(heap.is_empty());
}

TEST(Base, AddNonSorted)
{
    BinaryHeap heap(BinaryHeap::kMinHeap);
    heap.insert(5);
    cout << heap << endl;
    heap.insert(3);
    cout << heap << endl;
    heap.insert(2);
    cout << heap << endl;
    heap.insert(4);
    cout << heap << endl;
    heap.insert(1);
    cout << heap << endl;
    ASSERT_EQ(heap.count(), 5);
    ASSERT_FALSE(heap.is_empty());
}


class MinBinaryHeapTest : public ::testing::Test {
public:
    MinBinaryHeapTest() : heap_(BinaryHeap::kMinHeap)
    {}
    void SetUp()
    {
        for (int i = 0; i < 20; ++i)
            heap_.insert(i);
    }
protected:
    BinaryHeap heap_;
};

TEST_F(MinBinaryHeapTest, Contains)
{
    for (int i = 0; i < (int)heap_.count(); ++i)
        ASSERT_TRUE(heap_.contains(i));
}

TEST_F(MinBinaryHeapTest, Remove)
{
    for (int i = 0; i < 20; ++i) {
        ASSERT_NO_THROW(heap_.remove(i));
        ASSERT_FALSE(heap_.contains(i));
    }
}

TEST_F(MinBinaryHeapTest, Insert)
{
    ASSERT_NO_THROW(heap_.insert(10));
    ASSERT_EQ(heap_.count(), 21);
}

TEST_F(MinBinaryHeapTest, BFI)
{
    BinaryHeap::BreadthFirstIterator iterator = heap_.create_bft_iterator();
    cout << "{ ";
    for (int i = 0; i < (int)heap_.count(); ++i) {
        cout << *iterator << " ";
        ASSERT_EQ(*iterator, i);
        iterator.next();
    }
    cout << "}" << endl;
}

TEST_F(MinBinaryHeapTest, DFI)
{
    BinaryHeap::DepthFirstIterator iterator = heap_.create_dft_iterator();
    cout << "{ ";
    while (iterator || iterator.has_next()) {
        cout << *iterator << " ";
        iterator.next();
    }
    cout << "}" << endl;
}


class MaxBinaryHeapTest : public ::testing::Test {
public:
    MaxBinaryHeapTest() : heap_(BinaryHeap::kMaxHeap)
    {}
    void SetUp()
    {
        for (int i = 20; i > 0; --i)
            heap_.insert(i);
    }
protected:
    BinaryHeap heap_;
};

TEST_F(MaxBinaryHeapTest, Contains)
{
    for (int i = 1; i <= (int)heap_.count(); ++i)
        ASSERT_TRUE(heap_.contains(i));
}

TEST_F(MaxBinaryHeapTest, Remove)
{
    for (int i = 1; i <= 20; ++i) {
        ASSERT_NO_THROW(heap_.remove(i));
        ASSERT_FALSE(heap_.contains(i));
    }
}

TEST_F(MaxBinaryHeapTest, Insert)
{
    ASSERT_NO_THROW(heap_.insert(10));
    ASSERT_EQ(heap_.count(), 21);
}

TEST_F(MaxBinaryHeapTest, BFI)
{
    BinaryHeap::BreadthFirstIterator iterator = heap_.create_bft_iterator();
    cout << "{ ";
    for (int i = 20; i > 0; --i) {
        cout << *iterator << " ";
        ASSERT_EQ(*iterator, i);
        iterator.next();
    }
    cout << "}" << endl;
}

TEST_F(MaxBinaryHeapTest, DFI)
{
    BinaryHeap::DepthFirstIterator iterator = heap_.create_dft_iterator();
    cout << "{ ";
    while (iterator || iterator.has_next()) {
        cout << *iterator << " ";
        iterator.next();
    }
    cout << "}" << endl;
}
