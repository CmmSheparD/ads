#pragma once
#ifndef EXPRESSION_HH
#define EXPRESSION_HH

#include <string>
#include <vector>

#include "bst.hh"
#include "component.hh"

class Expression {
public:
    Expression() = default;
    Expression(BST<int, Component> tree) : tree_(tree) {}
    Expression(const Expression &other) = default;
    Expression(Expression &&other) = default;
    Expression &operator=(const Expression &other) {
        tree_ = other.tree_;
        return *this;
    }
    Expression &operator=(Expression &&other) {
        tree_ = other.tree_;
        return *this;
    }

    std::vector<std::string> visualize() const { return tree_.visualize(); }

    std::string asPrefix();
    std::string asInfix();
    std::string asPostfix();
private:
    std::string asInfixHelper(BST<int, Component>::PrefixIterator &sub);

    BST<int, Component> tree_;
};

#endif  // EXPRESSION_HH
