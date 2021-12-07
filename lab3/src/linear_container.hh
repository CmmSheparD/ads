#pragma once
#ifndef CONTAINER_HH
#define CONTAINER_HH

#include <memory>
#include <stdexcept>

namespace data_structs {

template<class T>
class LinearContainer {
public:
    LinearContainer() : head_(nullptr), tail_(nullptr), count_(0) {}
    virtual ~LinearContainer() = default;

    virtual void push(T data) = 0;
    void pop();

    T& peek();

    size_t size() { return count_; }
    bool is_empty() { return count_ == 0; }
protected:
    template<class U>
    struct Node;

    std::shared_ptr<Node<T>> head_;
    std::shared_ptr<Node<T>> tail_;
    size_t count_;
};


template<class T>
template<class U>
struct LinearContainer<T>::Node {
    Node(U data) : next(nullptr), data(data) {}

    std::shared_ptr<Node> next;
    U data;
};


template<class T>
void LinearContainer<T>::pop()
{
    if (count_ == 0)
        return;
    head_ = head_->next;
    if (!head_)
        tail_ = head_;
    --count_;
}


template<class T>
T& LinearContainer<T>::peek()
{
    if (count_ == 0)
        throw std::logic_error("Peeking on empty queue.");
    return head_->data;
}


}   // namespace data_structs

#endif  // CONTAINER_HH
