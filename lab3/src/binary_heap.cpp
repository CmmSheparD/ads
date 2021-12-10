#include "binary_heap.hh"

#include <cmath>
#include <memory>
#include <stdexcept>

#include "stack.hh"

namespace data_structs {

BinaryHeap::BreadthFirstIterator BinaryHeap::create_bft_iterator()
{
    return BreadthFirstIterator(*this);
}

BinaryHeap::DepthFirstIterator BinaryHeap::create_dft_iterator()
{
    return DepthFirstIterator(*this);
}


BinaryHeap::BinaryHeap(BinaryHeap& other)
    : BinaryHeap(other.type_)
{
    BreadthFirstIterator it = other.create_bft_iterator();
    while (it) {
        insert(*it);
        it.next();
    }
}

BinaryHeap::BinaryHeap(BinaryHeap&& other)
    : count_(other.count_), height_(other.height_), type_(other.type_)
{
    root_.swap(other.root_);
}


bool BinaryHeap::contains(int val)
{
    if (!root_)
        return false;
    std::shared_ptr<Node> cur = root_;
    return find_in_subtree(root_, val) != nullptr;
}

void BinaryHeap::insert(int val)
{
    if (count_ == 0) {
        root_ = std::shared_ptr<Node>(new Node(val));
        root_->root = true;
        height_ = 0;
    } else {
        RouteCode code;
        // if the last layer is full
        if (count_ == std::pow(2, height_ + 1) - 1) {
            code.code = 0;
            code.len = height_ + 1;
        } else {
            code.code = count_ - (std::pow(2, height_) - 1);
            code.len = height_;
        }
        TraverseResult end_point = traverse_by_code(code);
        end_point.ref = std::shared_ptr<Node>(new Node(val));
        end_point.ref->parent = end_point.parent;
        heapify(end_point.ref);
    }
    ++count_;
    recalculate_height();
}

void BinaryHeap::remove(int val)
{
    if (!root_)
        return;
    std::shared_ptr<Node> sought = find_in_subtree(root_, val);
    if (!sought) {
        return;
    } else if (count_ == 1) {
        root_.reset();
        count_ = 0;
        return;
    }
    RouteCode code = {count_ - (size_t)std::pow(2, height_), height_};
    TraverseResult res = traverse_by_code(code);
    std::shared_ptr<Node> last = res.ref;
    swap_nodes(sought, last);
    (sought->parent->right && sought->parent->right == sought
        ? sought->parent->right : sought->parent->left).reset();
    heapify(last);
    --count_;
    recalculate_height();
}


std::ostream &operator<<(std::ostream &os, BinaryHeap &h)
{
    BinaryHeap::BreadthFirstIterator it = h.create_bft_iterator();
    while (it || it.has_next()) {
        os << *it << ": ";
        if (it.cur_->left)
            os << it.cur_->left->data;
        if (it.cur_->right)
            os << ", " << it.cur_->right->data;
        os << "\n";
        it.next();
    }
    return os;
}


std::shared_ptr<BinaryHeap::Node> BinaryHeap::find_in_subtree(std::shared_ptr<Node> subroot, int val)
{
    std::shared_ptr<Node> sought = nullptr;
    if (subroot->data == val) {
        sought = subroot;
    } else if (!is_higher(val, subroot)) {
        if (subroot->left)
            sought = find_in_subtree(subroot->left, val);
        if (!sought && subroot->right)
            sought = find_in_subtree(subroot->right, val);
    }
    return sought;
}


BinaryHeap::TraverseResult BinaryHeap::traverse_by_code(BinaryHeap::RouteCode code)
{
    // Encode a route
    Stack<int> route;
    for (size_t i = 0; i < code.len; ++i) {
        route.push(code.code & 1);
        code.code = code.code >> 1;
    }
    // Traverse the route
    std::shared_ptr<Node> cur = root_;
    while (route.size() > 1) {
        cur = route.peek() == 0 ? cur->left : cur->right;
        route.pop();
    }
    TraverseResult res(route.peek() == 0 ? cur->left : cur->right);
    res.parent = cur;
    return res;
}


void BinaryHeap::swap_nodes(std::shared_ptr<Node> first, std::shared_ptr<Node> second)
{
    if (first == second)
        return;
    if (first->parent) {
        (first->parent->right == first
            ? first->parent->right : first->parent->left) = second;
    }
    if (second->parent) {
        (second->parent->right == second
            ? second->parent->right : second->parent->left) = first;
    }
    first->parent.swap(second->parent);
    if (first->left)
        first->left->parent = second;
    if (second->left)
        second->left->parent = first;
    first->left.swap(second->left);
    if (first->right)
        first->right->parent = second;
    if (second->right)
        second->right->parent = first;
    first->right.swap(second->right);
    bool tmp = second->root;
    second->root = first->root;
    first->root = tmp;
    if (first->root)
        root_ = first;
    else if (second->root)
        root_ = second;
}


void BinaryHeap::heapify(std::shared_ptr<Node> node)
{
    // If node's got no children, it can be only upsifted
    if (!node->left) {
        sift_up(node);
    } else if (node->root) {
        sift_down(node);
    } else {
        if (is_higher(node, node->parent))
            sift_up(node);
        else
            sift_down(node);
    }
}

void BinaryHeap::sift_up(std::shared_ptr<Node> node)
{
    while (!node->root && is_higher(node, node->parent)) {
        swap_nodes(node, node->parent);
    }
}

void BinaryHeap::sift_down(std::shared_ptr<Node> node)
{
    while (node->left) {
        std::shared_ptr<Node> to_swap;
        if (node->right && is_lower(node->left, node->right))
            to_swap = node->right;
        else
            to_swap = node->left;
        if (is_lower(node, to_swap)) {
            swap_nodes(node, to_swap);
        } else {
            break;
        }
    }
}


bool BinaryHeap::is_higher(int val, std::shared_ptr<Node> comparative)
{
    if (type_ == kMinHeap)
        return val < comparative->data;
    else
        return val > comparative->data;
}

bool BinaryHeap::is_higher(std::shared_ptr<Node> node, std::shared_ptr<Node> comparative)
{
    return is_higher(node->data, comparative);
}


bool BinaryHeap::is_lower(int val, std::shared_ptr<Node> comparative)
{
    if (type_ == kMinHeap)
        return val > comparative->data;
    else
        return val < comparative->data;
}

bool BinaryHeap::is_lower(std::shared_ptr<Node> node, std::shared_ptr<Node> comparative)
{
    return is_lower(node->data, comparative);
}


void BinaryHeap::recalculate_height()
{
    if (count_ < 2)
        height_ = 0;
    else
        height_ = (size_t)std::floor(std::log2l(count_));
}


int BinaryHeap::Iterator::operator*()
{
    if (!*this)
        throw std::logic_error("Dereferencing past-the-end iterator.");
    return cur_->data;
}


void BinaryHeap::Iterator::next()
{
    if (!cur_)
        throw std::out_of_range("Traversing with past-the-end iterator.");
    if (container_->is_empty()) {
        cur_ = nullptr;
    } else {
        cur_ = container_->peek();
        container_->pop();
        enqueue();
    }
}


void BinaryHeap::BreadthFirstIterator::enqueue()
{
    if (!cur_)
        return;
    if (cur_->left)
        container_->push(cur_->left);
    if (cur_->right)
        container_->push(cur_->right);
}


void BinaryHeap::DepthFirstIterator::enqueue()
{
    if (!cur_)
        return;
    if (cur_->right)
        container_->push(cur_->right);
    if  (cur_->left)
        container_->push(cur_->left);
}

}   // namespace data_structs
