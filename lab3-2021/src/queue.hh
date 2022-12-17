#pragma once
#ifndef QUEUE_HH
#define QUEUE_HH

#include <memory>
#include <stdexcept>

#include "linear_container.hh"

namespace data_structs {

template<class T>
class Queue : public LinearContainer<T> {
public:
    Queue() = default;
    ~Queue() = default;

    void push(T data);
};


template<class T>
void Queue<T>::push(T data)
{
    std::shared_ptr<typename LinearContainer<T>::Node<T>>
        node(new typename LinearContainer<T>::Node<T>(data));
    if (this->count_ == 0)
        this->head_ = node;
    else
        this->tail_->next = node;
    this->tail_ = node;
    ++this->count_;
}

}   // namespace data_structs

#endif  // QUEUE_HH
