#pragma once
#ifndef QUEUE_HH
#define QUEUE_HH

#include <memory>
#include <stdexcept>

namespace data_structs {

template<class T>
class Queue {
public:
    Queue() : first_(nullptr), last_(nullptr), count_(0) {}
    ~Queue() = default;

    void push(T data);
    void pop();

    T& front();

    size_t size();
    bool is_empty();
private:
    template<class U>
    struct Node;

    std::shared_ptr<Node<T>> first_;
    std::shared_ptr<Node<T>> last_;
    size_t count_;
};

template<class T>
template<class U>
struct Queue<T>::Node {
    Node(U data) : next(nullptr), data(data) {}

    std::shared_ptr<Node> next;
    U data;
};


template<class T>
void Queue<T>::push(T data)
{
    std::shared_ptr<Node<T>> node(new Node<T>(data));
    if (count_ == 0)
        first_ = node;
    else
        last_->next = node;
    last_ = node;
    ++count_;
}

template<class T>
void Queue<T>::pop()
{
    if (count_ == 0)
        return;
    first_ = first_->next;
    if (!first_)
        last_ = first_;
    --count_;
}


template<class T>
T& Queue<T>::front()
{
    if (count_ == 0)
        throw std::logic_error("Peeking on empty queue.");
    return first_->data;
}


template<class T>
size_t Queue<T>::size()
{
    return count_;
}

template<class T>
bool Queue<T>::is_empty()
{
    return count_ == 0;
}

}   // namespace data_structs

#endif  // QUEUE_HH
