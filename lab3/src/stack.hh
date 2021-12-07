#pragma once
#ifndef STACK_HH
#define STACK_HH

#include <memory>
#include <stdexcept>

#include "linear_container.hh"

namespace data_structs {

template<class T>
class Stack : public LinearContainer<T> {
public:
    Stack() = default;
    ~Stack() = default;

    void push(T data);
};

template<class T>
void Stack<T>::push(T data)
{
    std::shared_ptr<typename LinearContainer<T>::Node<T>>
        node(new typename LinearContainer<T>::Node<T>(data));
    if (this->count_ == 0)
        this->tail_ = node;
    else
        node->next = this->head_;
    this->head_ = node;
    ++this->count_;
}

}   // namespace data_structs

#endif  // STACK_HH
