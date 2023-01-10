#pragma once
#ifndef BINARY_HEAP_HH
#define BINARY_HEAP_HH

#include <ostream>
#include <memory>

#include "stack.hh"
#include "queue.hh"

namespace data_structs {

class BinaryHeap {
public:
    /*
     * Determines the sorting order for the nodes.
     */
    enum HeapType {kMinHeap, kMaxHeap};

    class Iterator;
    class BreadthFirstIterator;
    class DepthFirstIterator;

    BreadthFirstIterator create_bft_iterator();
    DepthFirstIterator create_dft_iterator();

    BinaryHeap(HeapType type)
        : root_(nullptr), count_(0), height_(0), type_(type)
    {}
    BinaryHeap(BinaryHeap& other);
    BinaryHeap(BinaryHeap&& other);

    ~BinaryHeap() = default;

    bool contains(int val);
    /*
     * What if every node in the heap would have its own code?
     * Let the code be a sequence of edges codes. And edges codes will
     * be 0 if it is descending to the left and 1 if it is descending
     * to the right:
     *        root
     *      /     \         Something like
     *    0        1         that, I gues..?
     *   / \      /
     * 00   01  10
     * Knowing the code of a node we could easily find its position
     * just following it. And this will help us find the position
     * to insert a new element. The amount of edge traversions to the
     * parent of future node equals height - 1 if the last layer of
     * nodes is incomplete and just height otherwise.
     * Let the amount of elements on the last layer be 0 if the last
     * layer is complete. Otherwise it is count - (pow(2, height) - 1).
     * The code of the future element will be equal to that number.
     * Length of the code equals height of the tree if the last layer
     * is incomplete and height + 1 otherwise.
     */
    void insert(int val);
    /*
     * Similalry, lets find the last element. Its code will always
     * be count - (pow(2, height)) and is always of height lenght.
     */
    void remove(int val);

    bool is_empty() { return count_ == 0; }
    size_t count() { return count_; }
    size_t height() { return height_; }
    HeapType type() { return type_; }

    friend std::ostream &operator<<(std::ostream &os, BinaryHeap &h);
private:
    struct Node;
    /*
     * The code of the binary route and its lenght.
     * Mind that route with code 0 is a root, processing this route
     * is not supported. Be careful!
     */
    struct RouteCode {
        size_t code;
        size_t len;
    };
    /*
     * Pointer to the parent of the node and a reference to the pointer
     * of the parent where an end point of the route would have been
     * stored if the end point would have existed. Might be a valid
     * pointer though, if the route leads to an existing node.
     */
    struct TraverseResult {
        TraverseResult(std::shared_ptr<Node> &end_point)
            : ref(end_point)
        {}

        std::shared_ptr<Node> parent;
        std::shared_ptr<Node> &ref;
    };

    // Recursively finds val in the subtree pointed by subroot
    std::shared_ptr<Node> find_in_subtree(std::shared_ptr<Node> subroot, int val);

    /*
     * Gets code, translates it to a binary route through
     * the heap, traverses it and returns reference to the pointer
     * that would hold that code.
     */
    TraverseResult traverse_by_code(RouteCode code);

    void swap_nodes(std::shared_ptr<Node> first, std::shared_ptr<Node> second);

    void heapify(std::shared_ptr<Node> node);
    void sift_up(std::shared_ptr<Node> node);
    void sift_down(std::shared_ptr<Node> node);
    /*
     * Tells if the node with the given value should be higher in the heap
     * then comparative node.
     */
    bool is_higher(int val, std::shared_ptr<Node> comparative);
    bool is_higher(std::shared_ptr<Node> node, std::shared_ptr<Node> comparative);
    /*
     * Tells if the node with the given value should be lower in the heap
     * then comparative node.
     */
    bool is_lower(int val, std::shared_ptr<Node> comparative);
    bool is_lower(std::shared_ptr<Node> node, std::shared_ptr<Node> comparative);

    void recalculate_height();

    std::shared_ptr<Node> root_;
    size_t count_;
    size_t height_;
    HeapType type_;
};


struct BinaryHeap::Node {
    Node(int val)
        : parent(nullptr), left(nullptr), right(nullptr),
        data(val), root(false)
    {}
    Node(const Node& other)
        : parent(nullptr), left(nullptr), right(nullptr),
        data(other.data), root(other.root)
    {}

    std::shared_ptr<Node> parent;
    /*
     * As the heap is packed from left to right, if the node has no
     * left child, it has no right either.
     */
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    int data;
    bool root;
};


class BinaryHeap::Iterator {
public:
    Iterator(std::shared_ptr<LinearContainer<std::shared_ptr<Node>>> container, std::shared_ptr<Node> &node)
        : container_(container), cur_(node)
    {}
    virtual ~Iterator() = default;

    int operator*();
    operator bool() const { return bool(cur_); }

    void next();
    bool has_next() { return !container_->is_empty(); }
    friend std::ostream &operator<<(std::ostream &os, BinaryHeap &h);
protected:
    virtual void enqueue() = 0;

    std::shared_ptr<LinearContainer<std::shared_ptr<Node>>> container_;
    std::shared_ptr<Node> cur_;
};


class BinaryHeap::BreadthFirstIterator : public BinaryHeap::Iterator {
public:
    BreadthFirstIterator(BinaryHeap& heap)
        : Iterator(std::shared_ptr<LinearContainer<std::shared_ptr<Node>>>(new Queue<std::shared_ptr<Node>>()), heap.root_)
    {
        enqueue();
    }
protected:
    void enqueue();
};


class BinaryHeap::DepthFirstIterator : public BinaryHeap::Iterator {
public:
    DepthFirstIterator(BinaryHeap& heap)
        : Iterator(std::shared_ptr<LinearContainer<std::shared_ptr<Node>>>(new Stack<std::shared_ptr<Node>>()), heap.root_)
    {
        enqueue();
    }
protected:
    void enqueue();
};

}   // namespace data_structs

#endif  // BINARY_HEAP_HH
