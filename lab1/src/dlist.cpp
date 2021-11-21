#include "dlist.hh"

#include <stdexcept>


namespace dlist {

void DLinkedList::push_back(int a)
{
    std::shared_ptr<Node> n(new Node(a));
    if (!tail_) {
        head_ = n;
    } else {
        tail_->next = n;
        n->prev = tail_;
    }
    tail_ = n;
    ++size_;
}

void DLinkedList::push_front(int a)
{
    std::shared_ptr<Node> n(new Node(a));
    if (!head_) {
        tail_ = n;
    } else {
        head_->prev = n;
        n->next = head_;
    }
    head_ = n;
    ++size_;
}

void DLinkedList::pop_back()
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

void DLinkedList::pop_front()
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


void DLinkedList::insert(int a, size_t i)
{
    if (i > size_) // if i == size_, node will be inserted after the last
        throw std::out_of_range("Index out of range.");
    else if (i == 0)
        return push_front(a);
    else if (i == size_)
        return push_back(a);
    std::shared_ptr<Node> n(new Node(a));
    std::shared_ptr<Node> tmp;
    if (i == size_ - 1) {
        tmp = tail_;
    } else {
        tmp = head_->next;
        for (size_t j = 1; j != i; ++j)
            tmp = tmp->next;
    }
    n->prev = tmp->prev;
    n->next = tmp;
    tmp->prev->next = n;
    tmp->prev = n;
    ++size_;
}

void DLinkedList::remove(size_t i)
{
    if (i >= size_) {
        throw std::out_of_range("Index out of range.");
    } else if (i == 0) {
        return pop_front();
    } else if (i == size_ - 1) {
        return pop_back();
    }
    std::shared_ptr<Node> tmp = head_->next;
    for (size_t j = 1; j != i; ++j)
        tmp = tmp->next;
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
    --size_;
}

int DLinkedList::at(size_t i)
{
    if (i >= size_) {
        throw std::out_of_range("Index out of range.");
    } else if (i == 0) {
        return head_->a;
    } else if (i == size_ - 1) {
        return tail_->a;
    }
    std::shared_ptr<Node> tmp = head_->next;
    for (size_t j = 1; j != i; ++j)
        tmp = tmp->next;
    return tmp->a;
}

void DLinkedList::set(size_t i, int a)
{
    if (i >= size_) {
        throw std::out_of_range("Index out of range.");
    } else if (i == 0) {
        head_->a = a;
        return;
    } else if (i == size_ - 1) {
        tail_->a = a;
        return;
    }
    std::shared_ptr<Node> tmp = head_->next;
    for (size_t j = 1; j != i; ++j)
        tmp = tmp->next;
    tmp->a = a;
}


void DLinkedList::clear()
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


void DLinkedList::insert(DLinkedList& list, size_t i)
{
    if (i > size_)
        throw std::out_of_range("Index out of range.");
    else if (list.is_empty())
        return;

    std::shared_ptr<Node> prev;
    std::shared_ptr<Node> cur;
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
        prev->next = list.head_;
        list.head_->prev = prev;
    } else {
        head_ = list.head_;
    }
    list.head_ = nullptr;
    // if cur is nullptr, then inserting into the tail of the list
    if (cur) {
        cur->prev = list.tail_;
        list.tail_->next = cur;
    } else {
        tail_ = list.tail_;
    }
    list.tail_ = nullptr;
    size_ += list.size_;
    list.size_ = 0;
}


std::ostream& operator<<(std::ostream& os, const DLinkedList& list)
{
    os << "Doubly Linked List: {";
    std::shared_ptr<DLinkedList::Node> tmp = list.head_;
    for (size_t i = 0; i != list.size_; ++i) {
        os << tmp->a;
        if (i != list.size_ - 1)
            os << ", ";
        tmp = tmp->next;
    }
    os << "}";
    return os;
}

}   // namespace dlist
