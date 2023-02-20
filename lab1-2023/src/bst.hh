#pragma once
#ifndef BST_HH
#define BST_HH

#include <memory>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

template<class Key, class Value>
class BST {
public:
    template<class IterationStrategy>
    class Iterator;

    class PrefixIterationStrategy;
    class InfixIterationStrategy;
    class PostfixIterationStrategy;
    class BreadthIterationStrategy;

    using PrefixIterator = Iterator<PrefixIterationStrategy>;
    using InfixIterator = Iterator<InfixIterationStrategy>;
    using PostfixIterator = Iterator<PostfixIterationStrategy>;
    using BreadthIterator = Iterator<BreadthIterationStrategy>;

    PrefixIterator prefixBegin() const { return PrefixIterator(root_); }
    InfixIterator infixBegin() const { return InfixIterator(root_); }
    PostfixIterator postfixBegin() const { return PostfixIterator(root_); }
    BreadthIterator breadthBegin() const { return BreadthIterator(root_); }

    BST() : root_(nullptr), n_(0) {}
    BST(const BST &other);
    BST(BST &&other);
    BST &operator=(const BST &other);
    BST &operator=(BST &&other);

    ~BST() = default;

    void insert(const Key &key, const Value &value);
    Value &find(Key key) const;
    void remove(Key key);

    Value min() const;
    Value max() const;

    size_t height() const;
    void clear();
private:
    template<class K, class V>
    struct Node_t;
    using Node = Node_t<Key, Value>;

    void deleteNode(std::shared_ptr<Node> &node);
    void height(std::shared_ptr<Node> node, size_t &h, size_t &max) const;

    std::shared_ptr<Node> root_;
    size_t n_;
};


class KeyError : public std::runtime_error {
public:
    KeyError() : runtime_error("No such key in the tree.") {}
};

class EmptyContainerError : public std::runtime_error {
public:
    EmptyContainerError()
        : runtime_error("Insufficient amount of elements for operation.")
    {}
};

class InvalidIteratorError : public std::logic_error {
public:
    InvalidIteratorError()
        : logic_error("Iterator have been invalidated before the operation.")
    {}
};

template<class Key, class Value>
BST<Key, Value>::BST(const BST &other) :
    BST()
{
    PrefixIterator it = other.prefixBegin();
    while (it) {
        insert(it.key(), *it);
        ++it;
    }
}

template<class Key, class Value>
BST<Key, Value>::BST(BST &&other) : n_(other.n_)
{
    root_.swap(other.root_);
}

template<class Key, class Value>
BST<Key, Value> &BST<Key, Value>::operator=(const BST<Key, Value> &other)
{
    PrefixIterator it = other.prefixBegin();
    clear();
    while (it) {
        insert(it.key(), *it);
        ++it;
    }
    return *this;
}

template<class Key, class Value>
BST<Key, Value> &BST<Key, Value>::operator=(BST<Key, Value> &&other)
{
    root_.swap(other.root_);
    return *this;
}

template<class Key, class Value>
void BST<Key, Value>::insert(const Key &key, const Value &value)
{
    if (n_ == 0) {
        root_ = std::make_shared<Node>(key, value);
    } else {
        std::shared_ptr<Node> prev = nullptr;
        std::shared_ptr<Node> cur = root_;
        std::shared_ptr<Node> *source = &root_;
        do {
            if (key == cur->key) {
                cur->data = value;
                prev = nullptr;
                cur = nullptr;
                source = nullptr;
            } else if (key < cur->key) {
                source = &cur->left;
                prev = cur;
                cur = cur->left;
            } else {
                source = &cur->right;
                prev = cur;
                cur = cur->right;
            }
        } while (cur != nullptr);
        if (source != nullptr) {
            *source = std::make_shared<Node>(key, value);
            (*source)->parent = prev;
        }
    }
    ++n_;
}

template<class Key, class Value>
Value &BST<Key, Value>::find(Key key) const
{
    std::shared_ptr<Node> cur = root_;
    while (cur != nullptr) {
        if (key == cur->key)
            return cur->data;
        else if (key < cur->key)
            cur = cur->left;
        else
            cur = cur->right;
    }
    throw KeyError();
}

template<class Key, class Value>
void BST<Key, Value>::remove(Key key)
{
    std::shared_ptr<Node> cur = root_;
    while (cur != nullptr) {
        if (key < cur->key) {
            cur = cur->left;
            continue;
        } else if (key > cur->key) {
            cur = cur->right;
            continue;
        }
        deleteNode(cur);
    }
    --n_;
}

template<class Key, class Value>
Value BST<Key, Value>::min() const
{
    if (n_ == 0) throw EmptyContainerError();
    std::shared_ptr<Node> cur = root_;
    while (cur->left != nullptr) cur = cur->left;
    return cur->data;
}

template<class Key, class Value>
Value BST<Key, Value>::max() const
{
    if (n_ == 0) throw EmptyContainerError();
    std::shared_ptr<Node> cur = root_;
    while (cur->right != nullptr) cur = cur->right;
    return cur->data;
}

template<class Key, class Value>
size_t BST<Key, Value>::height() const
{
    size_t h = 0;
    size_t maxHeight = 0;
    height(root_, h, maxHeight);
    return maxHeight;
}

template<class Key, class Value>
void BST<Key, Value>::clear()
{
    if (n_ == 0) return;
    n_ = 0;
    PostfixIterator it = postfixBegin();
    while (it) {
        Key key = it.key();
        ++it;
        remove(key);
    }
    root_ = nullptr;
}

template<class Key, class Value>
void BST<Key, Value>::deleteNode(std::shared_ptr<Node> &node)
{
    std::shared_ptr<Node> candidate = nullptr;
    if (node->left != nullptr && node->right != nullptr) {
        if (node->right->left == nullptr) {
            candidate = node->right;
        } else {
            candidate = node->right->left;
            while (candidate->left != nullptr) candidate = candidate->left;
        }
    } else if (node->left != nullptr) {
        candidate = node->left;
    } else if (node->right != nullptr) {
        candidate = node->right;
    }
    if (candidate != nullptr) {
        *node = *candidate;
        deleteNode(candidate);
    } else {
        if (node->parent != nullptr) {
            if (node->parent->left == node)
                node->parent->left = nullptr;
            else
                node->parent->right = nullptr;
        }
        if (root_ == node) root_ = nullptr;
    }
    node = nullptr;
}

template<class Key, class Value>
void BST<Key, Value>::height(std::shared_ptr<Node> node, size_t &h, size_t &max) const
{
    if (h > max) max = h;
    ++h;
    if (node->left != nullptr) height(node->left, h, max);
    if (node->right != nullptr) height(node->right, h, max);
    --h;
}

template<class Key, class Value>
template<class K, class V>
struct BST<Key, Value>::Node_t {
    Node_t(K key, V data)
        : data(data), key(key), parent(nullptr),
          left(nullptr), right(nullptr)
    {}
    Node_t(const Node_t &other) : Node_t(other.data, other.key)
    {}
    Node_t(Node_t &&other) = default;
    Node_t &operator=(const Node_t &other)
    {
        data = other.data;
        key = other.key;
        return *this;
    }
    Node_t &operator=(Node_t &&other) = default;
    ~Node_t() = default;

    // Data is probably bigger than key
    V data;
    K key;

    std::shared_ptr<Node_t> parent;
    std::shared_ptr<Node_t> left;
    std::shared_ptr<Node_t> right;
};

template<class Key, class Value>
template<class IterationStrategy>
class BST<Key, Value>::Iterator {
public:
    Iterator() = default;
    Iterator(std::shared_ptr<Node> start) : strategy_(start) {}
    Iterator(const Iterator &other) = default;
    Iterator(Iterator &&other) = default;
    Iterator &operator=(const Iterator &other) = default;
    Iterator &operator=(Iterator &&other) = default;

    ~Iterator() = default;

    Key key() const;
    Value value() const;
    std::pair<Key, Value> &operator*();
    const std::pair<Key, Value> &operator*() const;

    operator bool() const { return valid(); }

    Iterator &operator++();
    Iterator operator++(int);

    template<class K, class V>
    friend bool operator==(const Iterator &l, const Iterator &r);
    template<class K, class V>
    friend bool operator!=(const Iterator &l, const Iterator &r);
private:
    bool valid() const { return strategy_.valid(); }
    void validate() const
    {
        if (!valid())
            throw InvalidIteratorError();
    }

    IterationStrategy strategy_;
};

template<class Key, class Value>
template<class IterationStrategy>
Key BST<Key, Value>::Iterator<IterationStrategy>::key() const
{
    validate();
    return strategy_.get()->key;
}

template<class Key, class Value>
template<class IterationStrategy>
Value BST<Key, Value>::Iterator<IterationStrategy>::value() const
{
    validate();
    return strategy_.get()->data;
}

template<class Key, class Value>
template<class IterationStrategy>
std::pair<Key, Value> &BST<Key, Value>::Iterator<IterationStrategy>::operator*()
{
    validate();
    std::shared_ptr<Node> tmp = strategy_.get();
    return make_pair(tmp->key, tmp->value);
}

template<class Key, class Value>
template<class IterationStrategy>
const std::pair<Key, Value> &BST<Key, Value>::Iterator<IterationStrategy>::operator*() const
{
    validate();
    return *this;
}

template<class Key, class Value>
template<class IterationStrategy>
BST<Key, Value>::Iterator<IterationStrategy> &BST<Key, Value>::Iterator<IterationStrategy>::operator++()
{
    validate();
    strategy_.proceed();
    return *this;
}

template<class Key, class Value>
template<class IterationStrategy>
BST<Key, Value>::Iterator<IterationStrategy> BST<Key, Value>::Iterator<IterationStrategy>::operator++(int)
{
    validate();
    Iterator copy(*this);
    ++this;
    return copy;
}

template<class Key, class Value, class IterationStrategy>
bool operator==(const typename BST<Key, Value>::Iterator<IterationStrategy> &l,
                const typename BST<Key, Value>::Iterator<IterationStrategy> &r)
{
    if (!l.valid() && !r.valid())
        return true;
    else if (!l.valid() || !r.valid())
        return false;
    return l.strategy_.get() == r.strategy_.get();
}

template<class Key, class Value, class IterationStrategy>
bool operator!=(const typename BST<Key, Value>::Iterator<IterationStrategy> &l,
                const typename BST<Key, Value>::Iterator<IterationStrategy> &r)
{
    return !(l == r);
}

template<class Key, class Value>
class BST<Key, Value>::PrefixIterationStrategy {
public:
    PrefixIterationStrategy() : came_from_(kTop) {}
    PrefixIterationStrategy(std::shared_ptr<Node> &start)
        : cur_(start), came_from_(kTop)
    {}
    PrefixIterationStrategy(const PrefixIterationStrategy &other) = default;
    PrefixIterationStrategy(PrefixIterationStrategy &&other) = default;
    PrefixIterationStrategy &operator=(const PrefixIterationStrategy &other) = default;
    PrefixIterationStrategy &operator=(PrefixIterationStrategy &&other) = default;
    ~PrefixIterationStrategy() = default;

    void proceed();

    bool valid() const { return !cur_.expired(); }

    std::shared_ptr<Node> get() const { return cur_.lock(); }
private:
    enum Direction {kTop, kLeft, kRight};

    void detectAscentDirection(const std::shared_ptr<Node> &cur)
    {
        if (cur->parent && cur->parent->right == cur)
            came_from_ = kRight;
        else
            came_from_ = kLeft;
    }

    std::weak_ptr<Node> cur_;
    Direction came_from_;
};

template<class Key, class Value>
void BST<Key, Value>::PrefixIterationStrategy::proceed()
{
    std::shared_ptr<Node> lock = cur_.lock();
    bool keep_moving = false;
    do {
        keep_moving = false;
        switch (came_from_) {
        case kTop:
            if (lock->left) {
                lock = lock->left;
            } else if (lock->right) {
                lock = lock->right;
            } else {
                detectAscentDirection(lock);
                lock = lock->parent;
                if (lock)
                    keep_moving = true;
            }
            break;
        case kLeft:
            if (lock->right) {
                came_from_ = kTop;
                lock = lock->right;
            } else {
                detectAscentDirection(lock);
                lock = lock->parent;
                if (lock)
                    keep_moving = true;
            }
            break;
        case kRight:
            detectAscentDirection(lock);
            lock = lock->parent;
            if (lock)
                keep_moving = true;
        }
    } while (keep_moving);
    cur_ = lock;
}

template<class Key, class Value>
class BST<Key, Value>::InfixIterationStrategy {
public:
    InfixIterationStrategy() : came_from_(kTop) {}
    InfixIterationStrategy(std::shared_ptr<Node> &start);
    InfixIterationStrategy(const InfixIterationStrategy &other) = default;
    InfixIterationStrategy(InfixIterationStrategy &&other) = default;
    InfixIterationStrategy &operator=(const InfixIterationStrategy &other) = default;
    InfixIterationStrategy &operator=(InfixIterationStrategy &&other) = default;
    ~InfixIterationStrategy() = default;

    void proceed();

    bool valid() const { return !cur_.expired(); }

    std::shared_ptr<Node> get() const { return cur_.lock(); }
private:
    enum Direction {kTop, kLeft, kRight};

    void detectAscentDirection(const std::shared_ptr<Node> &cur)
    {
        if (cur->parent && cur->parent->right == cur)
            came_from_ = kRight;
        else
            came_from_ = kLeft;
    }

    std::weak_ptr<Node> cur_;
    Direction came_from_;
};

template<class Key, class Value>
BST<Key, Value>::InfixIterationStrategy::InfixIterationStrategy(
    std::shared_ptr<Node> &start)
        : cur_(start), came_from_(kTop)
{
    std::shared_ptr<Node> lock = cur_.lock();
    while (lock && lock->left) lock = lock->left;
    cur_ = lock;
}

template<class Key, class Value>
void BST<Key, Value>::InfixIterationStrategy::proceed()
{
    std::shared_ptr<Node> lock = cur_.lock();
    bool keep_moving = false;
    do {
        keep_moving = false;
        switch (came_from_) {
        case kTop:
            if (lock->left) {
                while (lock->left) lock = lock->left;
            } else if (lock->right) {
                lock = lock->right;
            } else {
                detectAscentDirection(lock);
                lock = lock->parent;
                if (lock && came_from_ != kLeft)
                    keep_moving = true;
            }
            break;
        case kLeft:
            if (lock->right) {
                came_from_ = kTop;
                lock = lock->right;
                if (lock->left || lock->right) keep_moving = true;
            } else {
                detectAscentDirection(lock);
                lock = lock->parent;
                if (lock && came_from_ == kRight) keep_moving = true;
            }
            break;
        case kRight:
            detectAscentDirection(lock);
            lock = lock->parent;
            if (lock && came_from_ == kRight) keep_moving = true;
        }
    } while (keep_moving);
    cur_ = lock;
}

template<class Key, class Value>
class BST<Key, Value>::PostfixIterationStrategy {
public:
    PostfixIterationStrategy() : came_from_(kTop) {}
    PostfixIterationStrategy(std::shared_ptr<Node> &start);
    PostfixIterationStrategy(const PostfixIterationStrategy &other) = default;
    PostfixIterationStrategy(PostfixIterationStrategy &&other) = default;
    PostfixIterationStrategy &operator=(const PostfixIterationStrategy &other) = default;
    PostfixIterationStrategy &operator=(PostfixIterationStrategy &&other) = default;
    ~PostfixIterationStrategy() = default;

    void proceed();

    bool valid() const { return !cur_.expired(); }

    std::shared_ptr<Node> get() const { return cur_.lock(); }
private:
    enum Direction {kTop, kLeft, kRight};

    void detectAscentDirection(const std::shared_ptr<Node> &cur)
    {
        if (cur->parent && cur->parent->right == cur)
            came_from_ = kRight;
        else
            came_from_ = kLeft;
    }

    std::weak_ptr<Node> cur_;
    Direction came_from_;
};

template<class Key, class Value>
BST<Key, Value>::PostfixIterationStrategy::PostfixIterationStrategy(
    std::shared_ptr<Node> &start)
        : cur_(start), came_from_(kTop)
{
    std::shared_ptr<Node> lock = cur_.lock();
    while (lock->left || lock->right) {
        while (lock->left) lock = lock->left;
        if (lock->right) lock = lock->right;
    }
    cur_ = lock;
}

template<class Key, class Value>
void BST<Key, Value>::PostfixIterationStrategy::proceed()
{
    std::shared_ptr<Node> lock = cur_.lock();
    bool keep_moving;
    do {
        keep_moving = false;
        switch (came_from_) {
        case kTop:
            if (lock->left || lock->right) {
                while (lock->left || lock->right) {
                    while (lock->left) lock = lock->left;
                    if (lock->right) lock = lock->right;
                }
            } else {
                detectAscentDirection(lock);
                lock = lock->parent;
                if (lock && came_from_ == kLeft && lock->right) {
                    lock = lock->right;
                    came_from_ = kTop;
                    if (lock->right || lock->left) keep_moving = true;
                }
            }
            break;
        case kLeft:
            if (lock->right) {
                came_from_ = kTop;
                lock = lock->right;
                if (lock->right || lock->left) keep_moving = true;
            } else {
                detectAscentDirection(lock);
                lock = lock->parent;
            }
            break;
        case kRight:
            detectAscentDirection(lock);
            lock = lock->parent;
            if (lock && came_from_ == kLeft && lock->right)
                keep_moving = true;
        }
    } while (keep_moving);
    cur_ = lock;
}

template<class Key, class Value>
class BST<Key, Value>::BreadthIterationStrategy {
public:
    BreadthIterationStrategy() = default;
    BreadthIterationStrategy(std::shared_ptr<Node> &start);
    BreadthIterationStrategy(const BreadthIterationStrategy &other) = default;
    BreadthIterationStrategy(BreadthIterationStrategy &&other) = default;
    BreadthIterationStrategy &operator=(const BreadthIterationStrategy &other) = default;
    BreadthIterationStrategy &operator=(BreadthIterationStrategy &&other) = default;
    ~BreadthIterationStrategy() = default;

    void proceed();

    bool valid() const { return !cur_.expired(); }

    std::shared_ptr<Node> get() const { return cur_.lock(); }
private:
    void enqueue()
    {
        std::shared_ptr<Node> lock = cur_.lock();
        if (lock->left) queue_.push(lock->left);
        if (lock->right) queue_.push(lock->right);
    }

    std::weak_ptr<Node> cur_;
    std::queue<std::weak_ptr<Node>> queue_;
};

template<class Key, class Value>
BST<Key, Value>::BreadthIterationStrategy::BreadthIterationStrategy(
    std::shared_ptr<Node> &start)
        : cur_(start)
{
    enqueue();
}

template<class Key, class Value>
void BST<Key, Value>::BreadthIterationStrategy::proceed()
{
    if (queue_.empty()) {
        cur_.reset();
    } else {
        cur_ = queue_.front();
        queue_.pop();
        enqueue();
    }
}

#endif  // BST_HH
