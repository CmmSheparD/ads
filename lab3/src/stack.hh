#pragma once
#ifndef STACK_HH
#define STACK_HH

#include <memory>
#include <stdexcept>

namespace data_structs {

template<class T>
class Stack {
public:
    Stack() : bottom_(nullptr), top_(nullptr), count_(0) {}
    ~Stack() = default;

    void push(T data);
    void pop();

    T& top();

    size_t size();
    bool is_empty();
private:
    template<class U>
    struct Node;

    std::shared_ptr<Node<T>> bottom_;
    std::shared_ptr<Node<T>> top_;
    size_t count_;
};

template<class T>
template<class U>
struct Stack<T>::Node {
    Node(U data) : next(nullptr), data(data) {}

    std::shared_ptr<Node> next;
    U data;
};


template<class T>
void Stack<T>::push(T data)
{
    std::shared_ptr<Node<T>> node(new Node<T>(data));
    if (count_ == 0)
        bottom_ = node;
    else
        node->next = top_;
    top_ = node;
    ++count_;
}

template<class T>
void Stack<T>::pop()
{
    if (count_ == 0)
        return;
    top_ = top_->next;
    if (!top_)
        bottom_ = top_;
    --count_;
}


template<class T>
T& Stack<T>::top()
{
    if (count_ == 0)
        throw std::logic_error("Peeking on empty stack.");
    return top_->data;
}


template<class T>
size_t Stack<T>::size()
{
    return count_;
}

template<class T>
bool Stack<T>::is_empty()
{
    return count_ == 0;
}

}   // namespace data_structs

#endif  // STACK_HH
