#include "parsing-table.hh"

#include <cctype>
#include <stdexcept>

#include "parsing-table.hh"

using namespace std;

list<Component> ParsingTable::constants_;
list<Component> ParsingTable::operators_;

bool ParsingTable::is_valid_name(const string &name)
{
    if (name.length() == 0)
        return false;
    if (isdigit(name[0]))
        return false;
    for (auto c : name) {
        if (!isgraph(c))
            return false;
    }
    return true;
}


void ParsingTable::register_constant(const std::string &name)
{
    if (!is_valid_name(name))
    constants_.push_back({name, 0, Component::kOperand});
}

void ParsingTable::register_operator(const std::string &name, int priority)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    operators_.push_back({name, priority, Component::kOperator});
}


bool ParsingTable::is_constant(const string &name)
{
    if (!is_valid_name(name))
        return false;
    for (auto &c : constants_) {
        if (c.str() == name)
            return true;
    }
    return false;
}

bool ParsingTable::is_operator(const std::string &name)
{
    if (!is_valid_name(name))
        return false;
    for (auto &c : operators_) {
        if (c.str() == name)
            return true;
    }
    return false;
}

Component ParsingTable::get_constant(const std::string &name)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    for (auto &c : constants_) {
        if (name == c.str())
            return c;
    }
    throw NameSearchError(name);
}

Component ParsingTable::get_operator(const std::string &name)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    for (auto &op : operators_) {
        if (name == op.str())
            return op;
    }
    throw NameSearchError(name);
}
