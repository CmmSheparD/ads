#pragma once
#ifndef EXPRESSION_HH
#define EXPRESSION_HH

#include <string>
#include <vector>

#include "bst.hh"
#include "component.hh"

class Expression {
public:
    Expression(BST<int, Component> tree) : tree_(tree) {}

    std::string asPrefix();
    std::string asInfix();
    std::string asPostfix();
private:
    std::string asInfixHelper(BST<int, Component>::InfixIterator sub);

    BST<int, Component> tree_;
};

#endif  // EXPRESSION_HH
