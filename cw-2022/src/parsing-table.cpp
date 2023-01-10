#include "parsing-table.hh"

#include <cctype>
#include <stdexcept>

#include "parsing-table.hh"

using namespace std;

namespace parsing {

list<ParsingTable::ConstantEntry> ParsingTable::constants_;
list<ParsingTable::UnaryOperatorEntry> ParsingTable::unary_operators_;
list<ParsingTable::BinaryOperatorEntry> ParsingTable::binary_operators_;


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


void ParsingTable::register_constant(const string &name, const double value)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    constants_.push_back({name, value});
}

void ParsingTable::register_unary(const string &name, function<double (double)> f)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    if (!f)
        throw invalid_argument("Operator cannot be null.");
    unary_operators_.push_back({name, f});
}

void ParsingTable::register_binary(const string &name, function<double (double, double)> f, unsigned order)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    if (!f)
        throw invalid_argument("Operator cannot be null.");
    binary_operators_.push_back({name, f, order});
}


bool ParsingTable::is_constant(const string &name)
{
    if (!is_valid_name(name))
        return false;
    for (auto &c : constants_) {
        if (c.data.str() == name)
            return true;
    }
    return false;
}

bool ParsingTable::is_unary_operator(const string &name)
{
    if (!is_valid_name(name))
        return false;
    for (auto &c : unary_operators_) {
        if (c.data.repr() == name)
            return true;
    }
    return false;
}

bool ParsingTable::is_binary_operator(const string &name)
{
    if (!is_valid_name(name))
        return false;
    for (auto &c : binary_operators_) {
        if (c.data.repr() == name)
            return true;
    }
    return false;
}


shared_ptr<calculation::Constant> ParsingTable::get_constant(const string &name)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    for (auto &c : constants_) {
        if (name == c.data.str())
            return shared_ptr<calculation::Constant>(new calculation::Constant(c.data));
    }
    throw NameSearchError(name);
}

shared_ptr<calculation::UnaryOperator> ParsingTable::get_unary_operator(const string &name)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    for (auto &op : unary_operators_) {
        if (name == op.data.repr())
            return shared_ptr<calculation::UnaryOperator>(new calculation::UnaryOperator(op.data));
    }
    throw NameSearchError(name);
}

shared_ptr<calculation::BinaryOperator> ParsingTable::get_binary_operator(const string &name)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    for (auto &op : binary_operators_) {
        if (name == op.data.repr())
            return shared_ptr<calculation::BinaryOperator>(new calculation::BinaryOperator(op.data));
    }
    throw NameSearchError(name);
}

}   // namespace parsing
