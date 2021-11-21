#pragma once
#ifndef DLIST_HH
#define DLIST_HH

#include <ostream>
#include <memory>


namespace dlist {

class DLinkedList {
public:
    DLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
    ~DLinkedList() { clear(); }

    void push_back(int a);
    void push_front(int a);
    void pop_back();
    void pop_front();

    void insert(int a, size_t i);
    void remove(size_t i);
    int at(size_t i);
    void set(size_t i, int a);

    size_t get_size() { return size_; }
    void clear();
    bool is_empty() { return size_ == 0; }

    void insert(DLinkedList& list, size_t i);

    friend std::ostream& operator<<(std::ostream& os, const DLinkedList& list);
private:
    struct Node;

    std::shared_ptr<Node> head_;
    std::shared_ptr<Node> tail_;

    size_t size_;
};


struct DLinkedList::Node {
    Node(int a) : prev(nullptr), next(nullptr), a(a) {}

    std::shared_ptr<Node> prev;
    std::shared_ptr<Node> next;

    int a;
};

}	// namespace dlist

#endif	// DLIST_HH
