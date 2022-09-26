#pragma once
#ifndef DLIST_HH
#define DLIST_HH

#include <ostream>
#include <memory>

namespace dlist {

template<class T>
class DLinkedList {
public:
    DLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
    DLinkedList(const DLinkedList &src);
    DLinkedList(DLinkedList &&src);
    ~DLinkedList() { clear(); }

    void push_back(T value);
    void push_front(T value);
    void pop_back();
    void pop_front();

    void insert(T value, size_t i);
    void remove(size_t i);
    T &at(size_t i);
    void set(size_t i, T value);

    size_t get_size() { return size_; }
    void clear();
    bool is_empty() { return size_ == 0; }

    void insert(DLinkedList<T> &list, size_t i);

private:
    template<class U>
    struct Node;

    std::shared_ptr<Node<T>> head_;
    std::shared_ptr<Node<T>> tail_;

    size_t size_;
};


template<class T>
template<class U>
struct DLinkedList<T>::Node {
    Node(U value) : prev(nullptr), next(nullptr), value(value) {}

    std::shared_ptr<Node> prev;
    std::shared_ptr<Node> next;

    U value;
};


template<class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &src) : size_(0)
{
    std::shared_ptr<Node<T>> cur = src.head_;
    while (cur) {
        push_back(cur->value);
        cur = cur->next;
    }
}

template<class T>
DLinkedList<T>::DLinkedList(DLinkedList<T> &&src) : size_(src.size_)
{
    head_.swap(src.head_);
    tail_.swap(src.tail_);
}

template<class T>
void DLinkedList<T>::push_back(T value)
{
    std::shared_ptr<Node<T>> node(new Node<T>(value));
    if (!tail_) {
        head_ = node;
    } else {
        tail_->next = node;
        node->prev = tail_;
    }
    tail_ = node;
    ++size_;
}

template<class T>
void DLinkedList<T>::push_front(T value)
{
    std::shared_ptr<Node<T>> node(new Node<T>(value));
    if (!head_) {
        tail_ = node;
    } else {
        head_->prev = node;
        node->next = head_;
    }
    head_ = node;
    ++size_;
}

template<class T>
void DLinkedList<T>::pop_back()
{
    if (!is_empty()) {
        tail_ = tail_->prev;
        if (!tail_)
            head_ = nullptr;
        else
            tail_->next = nullptr;
        --size_;
    }
}

template<class T>
void DLinkedList<T>::pop_front()
{
    if (!is_empty()) {
        head_ = head_->next;
        if (!head_)
            tail_ = nullptr;
        else
            head_->prev = nullptr;
        --size_;
    }
}

template<class T>
void DLinkedList<T>::insert(T value, size_t i)
{
    if (i > size_) // if i == size_, node will be inserted after the last
        throw std::out_of_range("Index out of range.");
    else if (i == 0)
        return push_front(value);
    else if (i == size_)
        return push_back(value);
    std::shared_ptr<Node<T>> node(new Node<T>(value));
    std::shared_ptr<Node<T>> tmp;
    if (i == size_ - 1) {
        tmp = tail_;
    } else {
        tmp = head_->next;
        for (size_t j = 1; j != i; ++j)
            tmp = tmp->next;
    }
    node->prev = tmp->prev;
    node->next = tmp;
    tmp->prev->next = node;
    tmp->prev = node;
    ++size_;
}

template<class T>
void DLinkedList<T>::remove(size_t i)
{
    if (i >= size_) {
        throw std::out_of_range("Index out of range.");
    } else if (i == 0) {
        return pop_front();
    } else if (i == size_ - 1) {
        return pop_back();
    }
    std::shared_ptr<Node<T>> tmp = head_->next;
    for (size_t j = 1; j != i; ++j)
        tmp = tmp->next;
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
    --size_;
}

template<class T>
T &DLinkedList<T>::at(size_t i)
{
    if (i >= size_) {
        throw std::out_of_range("Index out of range.");
    } else if (i == 0) {
        return head_->value;
    } else if (i == size_ - 1) {
        return tail_->value;
    }
    std::shared_ptr<Node<T>> tmp = head_->next;
    for (size_t j = 1; j != i; ++j)
        tmp = tmp->next;
    return tmp->value;
}

template<class T>
void DLinkedList<T>::set(size_t i, T value)
{
    if (i >= size_) {
        throw std::out_of_range("Index out of range.");
    } else if (i == 0) {
        head_->value = value;
        return;
    } else if (i == size_ - 1) {
        tail_->value = value;
        return;
    }
    std::shared_ptr<Node<T>> tmp = head_->next;
    for (size_t j = 1; j != i; ++j)
        tmp = tmp->next;
    tmp->value = value;
}

template<class T>
void DLinkedList<T>::clear()
{
    if (is_empty())
        return;
    tail_.reset();
    while (head_) {
       head_->prev.reset();
       head_ = head_->next;
    }
    size_ = 0;
}

template<class T>
void DLinkedList<T>::insert(DLinkedList<T> &list, size_t i)
{
    if (i > size_)
        throw std::out_of_range("Index out of range.");
    else if (list.is_empty())
        return;

    DLinkedList copy = list;
    std::shared_ptr<Node<T>> prev;
    std::shared_ptr<Node<T>> cur;
    if (i == size_) {
        prev = tail_;
        cur = nullptr;
    } else {
        prev = nullptr;
        cur = head_;
        for (size_t j = 0; j != i; ++j) {
            prev = cur;
            cur = cur->next;
        }
    }
    // if prev is nullptr, then inserting into the head of the list
    if (prev) {
        prev->next = copy.head_;
        copy.head_->prev = prev;
    } else {
        head_ = copy.head_;
    }
    // if cur is nullptr, then inserting into the tail of the list
    if (cur) {
        cur->prev = copy.tail_;
        copy.tail_->next = cur;
    } else {
        tail_ = copy.tail_;
    }
    size_ += copy.size_;
}

}	// namespace dlist

#endif	// DLIST_HH
