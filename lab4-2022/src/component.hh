#pragma once
#ifndef COMPONENT_HH
#define COMPONENT_HH

#include <ostream>
#include <string>

class Component {
public:
    enum Type {
        kOperand,
        kOperator,
        kSpecial
    };

    Component() = default;
    Component(std::string repr, int priority, Type type)
        : repr_(repr), priority_(priority), type_(type)
    {}
    Component &operator=(const Component &other) = default;

    int getPriority() const { return priority_; }
    void setPriority(int priority);

    Type getType() const { return type_; }

    std::string str() const { return repr_; }
protected:
    std::string repr_;
    int priority_;
    Type type_;
};

std::ostream &operator<<(std::ostream &os, const Component &c);

#endif  // COMPONENT_HH
