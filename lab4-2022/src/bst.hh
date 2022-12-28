#pragma once
#ifndef BST_HH
#define BST_HH

#include <cmath>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template<class Key, class Value>
class BST {
public:
    class Iterator;
    class InfixIterator;
    class PrefixIterator;
    class PostfixIterator;
    class BreadthIterator;

    InfixIterator infixBegin() const { return InfixIterator(root_); }
    PrefixIterator prefixBegin() const { return PrefixIterator(root_); }
    PostfixIterator postfixBegin() const { return PostfixIterator(root_); }
    BreadthIterator breadthBegin() const { return BreadthIterator(root_); }

    BST() : root_(nullptr), n_(0) {}
    BST(const BST &other);
    BST(BST &&other);
    
    ~BST() = default;

    void insert(const Key &key, const Value &value);
    Value &find(Key key) const;
    void remove(Key key);

    Value min() const;
    Value max() const;

    Value findNext(Key key) const;
    Value findPrev(Key key) const;

    size_t height() const;
    std::vector<std::string> visualize() const;
private:
    template<class K, class V>
    struct Node;

    void deleteNode(std::shared_ptr<Node<Key, Value>> node);
    void height(std::shared_ptr<Node<Key, Value>> node, size_t &h, size_t &max) const;

    std::shared_ptr<Node<Key, Value>> root_;
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

template<class Key, class Value>
BST<Key, Value>::BST(const BST &other)
{
    PrefixIterator it = other.prefixBegin();
    insert(it.key(), *it);
}

template<class Key, class Value>
BST<Key, Value>::BST(BST &&other) : n_(other.n_)
{
    root_.swap(other.root_);
}

template<class Key, class Value>
void BST<Key, Value>::insert(const Key &key, const Value &value)
{
    if (n_ == 0) {
        root_ = std::make_shared<Node<Key, Value>>(key, value);
    } else {
        std::shared_ptr<Node<Key, Value>> cur = root_;
        std::shared_ptr<Node<Key, Value>> *source = &root_;
        do {
            if (key == cur->key) {
                cur->data = value;
                cur = nullptr;
                source = nullptr;
            } else if (key < cur->key) {
                source = &cur->left;
                cur = cur->left;
            } else {
                source = &cur->right;
                cur = cur->right;
            }
        } while (cur != nullptr);
        if (source != nullptr)
            *source = std::make_shared<Node<Key, Value>>(key, value);
    }
    ++n_;
}

template<class Key, class Value>
Value &BST<Key, Value>::find(Key key) const
{
    std::shared_ptr<Node<Key, Value>> cur = root_;
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
    std::shared_ptr<Node<Key, Value>> cur = root_;
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
    std::shared_ptr<Node<Key, Value>> cur = root_;
    while (cur->left != nullptr) cur = cur->left;
    return cur->data;
}

template<class Key, class Value>
Value BST<Key, Value>::max() const
{
    if (n_ == 0) throw EmptyContainerError();
    std::shared_ptr<Node<Key, Value>> cur = root_;
    while (cur->right != nullptr) cur = cur->right;
    return cur->data;
}

template<class Key, class Value>
Value BST<Key, Value>::findNext(Key key) const
{
    if (n_ == 0) throw EmptyContainerError();
    InfixIterator it = infixBegin();
    while (it && it.key() < key) ++it;
    if (it) return *++it;
    throw KeyError();
}

template<class Key, class Value>
Value BST<Key, Value>::findPrev(Key key) const
{
    if (n_ == 0) throw EmptyContainerError();
    InfixIterator it = infixBegin();
    while (it && it.key() < key) ++it;
    if (it) return *--it;
    throw KeyError();
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
std::vector<std::string> BST<Key, Value>::visualize() const
{
    if (n_ == 0) throw EmptyContainerError();
    size_t h = height();
    std::vector<std::shared_ptr<Node<Key, Value>>> drawQueue;
    drawQueue.push_back(root_);
    size_t n = std::pow(2, h + 1) - 1;
    for (size_t i = 0; i < n; ++i) {
        if (drawQueue[i] == nullptr) {
            drawQueue.push_back(nullptr);
            drawQueue.push_back(nullptr);
        } else {
            drawQueue.push_back(drawQueue[i]->left);
            drawQueue.push_back(drawQueue[i]->right);
        }
    }
    // Transfrom every node into string and distribute them over the layers
    size_t node_len = 0;
    std::vector<std::vector<std::string>> strings;
    n = 0;
    for (size_t i = 0; i < h + 1; ++i) {
        strings.push_back({});
        for (size_t j = 0; j < pow(2, i); ++j) {
            std::stringstream fmt;
            if (drawQueue[n] == nullptr) {
                fmt << "";
            } else {
                fmt << drawQueue[n]->key;
                size_t l = fmt.str().length();
                if (l > node_len) node_len = l;
            }
            strings[i].push_back(fmt.str());
            ++n;
        }
    }
    // Max width that a layer can achieve
    size_t max_width = node_len * strings.rbegin()->size()
                       + strings.rbegin()->size() - 1;
    std::vector<string> res;
    /*
     * On every layer get the spacer length and join every string on the layer
     * and apply spacing plus the difference between the max node length and
     * current node length
     */
    for (std::vector<string> layer : strings) {
        std::string spacer(
            (max_width - node_len * layer.size()) / (layer.size() + 1),
            ' ');
        std::string buf;
        for (std::string node : layer) {
            std::string aux_spacer((node_len - node.length()) / 2, ' ');
            buf += spacer + aux_spacer + node + aux_spacer;
        }
        res.push_back(buf);
    }
    return res;
}

template<class Key, class Value>
void BST<Key, Value>::deleteNode(std::shared_ptr<Node<Key, Value>> node)
{
    std::shared_ptr<Node<Key, Value>> candidate = nullptr;
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
}

template<class Key, class Value>
void BST<Key, Value>::height(std::shared_ptr<Node<Key, Value>> node, size_t &h, size_t &max) const
{
    if (h > max) max = h;
    ++h;
    if (node->left != nullptr) height(node->left, h, max);
    if (node->right != nullptr) height(node->right, h, max);
    --h;
}

template<class Key, class Value>
template<class K, class V>
struct BST<Key, Value>::Node {
    Node(K key, V data)
        : data(data), key(key), parent(nullptr),
          left(nullptr), right(nullptr)
    {}
    Node(const Node &other) : Node(other.data, other.key)
    {}
    Node(Node &&other) = default;
    Node &operator=(const Node &other)
    {
        data = other.data;
        key = other.key;
        return *this;
    }
    Node &operator=(Node &&other) = default;
    ~Node() = default;

    // Data is probably bigger than key
    V data;
    K key;

    std::shared_ptr<Node> parent;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

template<class Key, class Value>
class BST<Key, Value>::Iterator {
public:
    virtual ~Iterator() = default;

    Value operator*() const { return (*cur_)->data; }
    Key key() const { return (*cur_)->key; }
    operator bool() const { return cur_ < order_.end() && cur_ >= order_.begin(); }

    Iterator &operator++()
    {
        ++cur_;
        return *this;
    }
    Iterator &operator--()
    {
        --cur_;
        return *this;
    }

    friend bool operator==(const Iterator &l, const Iterator &r)
    {
        return *l.cur_ == *r.cur_;
    }
    friend bool operator!=(const Iterator &l, const Iterator &r)
    {
        return *l.cur_ != *r.cur_;
    }
protected:
    virtual void enqueue(std::shared_ptr<Node<Key, Value>> node) = 0;

    std::vector<std::shared_ptr<Node<Key, Value>>> order_;
    typename std::vector<std::shared_ptr<Node<Key, Value>>>::iterator cur_;
};


template<class Key, class Value>
class BST<Key, Value>::InfixIterator : public Iterator {
public:
    InfixIterator(std::shared_ptr<Node<Key, Value>> root)
    {
        enqueue(root);
        this->cur_ = this->order_.begin();
    }
protected:
    virtual void enqueue(std::shared_ptr<Node<Key, Value>> node)
    {
        if (node->left != nullptr) enqueue(node->left);
        this->order_.push_back(node);
        if (node->right != nullptr) enqueue(node->right);
    }
};

template<class Key, class Value>
class BST<Key, Value>::PrefixIterator : public Iterator {
public:
    PrefixIterator(std::shared_ptr<Node<Key, Value>> root)
    {
        enqueue(root);
        this->cur_ = this->order_.begin();
    }
protected:
    virtual void enqueue(std::shared_ptr<Node<Key, Value>> node)
    {
        this->order_.push_back(node);
        if (node->left != nullptr) enqueue(node->left);
        if (node->right != nullptr) enqueue(node->right);
    }
};

template<class Key, class Value>
class BST<Key, Value>::PostfixIterator : public Iterator {
public:
    PostfixIterator(std::shared_ptr<Node<Key, Value>> root)
    {
        enqueue(root);
        this->cur_ = this->order_.begin();
    }
protected:
    virtual void enqueue(std::shared_ptr<Node<Key, Value>> node)
    {
        if (node->left != nullptr) enqueue(node->left);
        if (node->right != nullptr) enqueue(node->right);
        this->order_.push_back(node);
    }
};

template<class Key, class Value>
class BST<Key, Value>::BreadthIterator : public Iterator {
public:
    BreadthIterator(std::shared_ptr<Node<Key, Value>> root)
    {
        enqueue(root);
        this->cur_ = this->order_.begin();
    }
protected:
    virtual void enqueue(std::shared_ptr<Node<Key, Value>> node)
    {
        this->order_.push_back(node);
        for (size_t i = 0; i < this->order_.size(); ++i) {
            if (this->order_[i]->left != nullptr)
                this->order_.push_back(this->order_[i]->left);
            if (this->order_[i]->right != nullptr)
                this->order_.push_back(this->order_[i]->right);
        }
    }
};

#endif  // BST_HH
