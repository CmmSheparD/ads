#include "dlist.hh"

#include <stdexcept>


namespace dlist {


DLinkedList::DLinkedList(const DLinkedList& src) : size_(0)
{
    std::shared_ptr<Node> cur = src.head_;
    while (cur) {
        push_back(cur->value);
        cur = cur->next;
    }
}


void DLinkedList::push_back(int value)
{
    std::shared_ptr<Node> node(new Node(value));
    if (!tail_) {
        head_ = node;
    } else {
        tail_->next = node;
        node->prev = tail_;
    }
    tail_ = node;
    ++size_;
}

void DLinkedList::push_front(int value)
{
    std::shared_ptr<Node> node(new Node(value));
    if (!head_) {
        tail_ = node;
    } else {
        head_->prev = node;
        node->next = head_;
    }
    head_ = node;
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


void DLinkedList::insert(int value, size_t i)
{
    if (i > size_) // if i == size_, node will be inserted after the last
        throw std::out_of_range("Index out of range.");
    else if (i == 0)
        return push_front(value);
    else if (i == size_)
        return push_back(value);
    std::shared_ptr<Node> node(new Node(value));
    std::shared_ptr<Node> tmp;
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
        return head_->value;
    } else if (i == size_ - 1) {
        return tail_->value;
    }
    std::shared_ptr<Node> tmp = head_->next;
    for (size_t j = 1; j != i; ++j)
        tmp = tmp->next;
    return tmp->value;
}

void DLinkedList::set(size_t i, int value)
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
    std::shared_ptr<Node> tmp = head_->next;
    for (size_t j = 1; j != i; ++j)
        tmp = tmp->next;
    tmp->value = value;
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

    DLinkedList copy = list;
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


std::ostream& operator<<(std::ostream& outstream, const DLinkedList& list)
{
    outstream << "Doubly Linked List: {";
    std::shared_ptr<DLinkedList::Node> tmp = list.head_;
    for (size_t i = 0; i != list.size_; ++i) {
        outstream << tmp->value;
        if (i != list.size_ - 1)
            outstream << ", ";
        tmp = tmp->next;
    }
    outstream << "}";
    return outstream;
}

}   // namespace dlist
