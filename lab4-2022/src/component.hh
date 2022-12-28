#pragma once
#ifndef COMPONENT_HH
#define COMPONENT_HH

#include <string>

class Component {
public:
    enum Type {
        kOperand,
        kOperator
    };

    Component(std::string repr, int priority, Type type)
        : repr_(repr), priority_(priority), type_(type)
    {}

    int getPriority() { return priority_; }
    void setPriority(int priority);

    Type getType() { return type_; }

    std::string str() { return repr_; }
protected:
    std::string repr_;
    int priority_;
    Type type_;
};

#endif  // COMPONENT_HH
