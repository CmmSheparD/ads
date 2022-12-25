#include "parsing.hh"

#include <cmath>
#include <functional>
#include <list>
#include <memory>
#include <string>

#include "calculation-tree.hh"
#include "parsing-exceptions.hh"
#include "parsing-table.hh"

namespace parsing {

using table = ParsingTable;

using namespace std;

using calculation::Operand;
using calculation::Constant;
using calculation::Expression;

using calculation::Operator;
using calculation::UnaryOperator;
using calculation::BinaryOperator;


void init_table()
{
    table::register_constant("pi", 3.141592653589793);
    table::register_constant("e",  2.718281828459045);

    table::register_unary("-", negate<double>());

    table::register_binary("^", (double (*)(double, double))pow, 0);
    table::register_binary("*", multiplies<double>(), 1);
    table::register_binary("/", divides<double>(), 1);
    table::register_binary("+", plus<double>(), 2);
    table::register_binary("-", minus<double>(), 2);
}


size_t skip_spaces(const string &str, size_t start)
{
    const size_t len = str.size();
    size_t pos = start;
    while (pos < len && isspace(str[pos]))
        ++pos;
    return pos;
}

shared_ptr<Constant> parse_value(const string &str, size_t &start)
{
    size_t processed = 0;
    double val;
    try {
        string buffer(str, start, str.size() - start);
        val = stod(buffer, &processed);
    } catch (out_of_range &) {
        throw TooBigNumber(start);
    }
    shared_ptr<Constant> res = make_shared<Constant>(
        val,
        string(str, start, processed)
    );
    start += processed;
    return res;
}

shared_ptr<Constant> parse_constant(const string &str, size_t &start)
{
    const size_t len = str.length();
    size_t pos = start;
    string copy;
    copy += str[pos];
    while (!table::is_constant(copy)) {
        if (++pos == len)
            throw UnexpectedEndOfExpression(pos);
        copy += str[pos];
    }
    start = pos + 1;
    return table::get_constant(copy);
}

shared_ptr<UnaryOperator> parse_unary(const string &str, size_t &start)
{
    const size_t len = str.length();
    size_t pos = start;
    string copy;
    copy += str[pos];
    while (!table::is_unary_operator(copy)) {
        if (++pos == len)
            throw UnexpectedEndOfExpression(pos);
        copy += str[pos];
    }
    start = pos + 1;
    return table::get_unary_operator(copy);
}

shared_ptr<Operand> parse_operand(const string &str, size_t &start)
{
    const size_t len = str.length();
    if (start >= len)
        throw UnexpectedEndOfExpression(start);
    size_t pos = skip_spaces(str, start);
    if (pos == len)
        throw OperandExpectationUnsatisfied(pos);

    size_t backup_pos = pos;
    shared_ptr<Operand> res;
    if (str[pos] == '(') {
        size_t counter = 1;
        for (++pos; pos < len && counter > 0; ++pos) {
            if (str[pos] == '(')
                ++counter;
            else if (str[pos] == ')')
                --counter;
        }
        if (pos == len && counter != 0)
            throw UnexpectedEndOfExpression(pos);
        string inner(str, backup_pos + 1, pos - backup_pos - 2);
        try {
            res = parse_infix_expression(inner);
        } catch (ParserError &e) {
            // Restoring absolute position in the string and re-throwing
            e.position += backup_pos + 1;
            throw e;
        }
    } else if (table::is_starting_digit(str[pos])) {
        res = parse_value(str, pos);
    } else {
        try {
            shared_ptr<UnaryOperator> tmp = parse_unary(str, pos);
            tmp->set_operand(parse_operand(str, pos));

            shared_ptr<Expression> exp(new Expression);
            exp->set_root(tmp);
            res = exp;
        } catch (const ParserError&) {
            pos = backup_pos;
            try {
                res = parse_constant(str, pos);
            } catch (const UnexpectedEndOfExpression&) {
                throw OperandExpectationUnsatisfied(backup_pos);
            }
        }
    }
    start = pos;
    return res;
}


shared_ptr<BinaryOperator> parse_operator(const string &str, size_t &start)
{
    const size_t len = str.length();
    if (start >= len)
        throw UnexpectedEndOfExpression(len);
    size_t pos = skip_spaces(str, start);
    if (pos == len)
        throw BinaryExpectationUnsatisfied(start);

    string copy;
    copy += str[pos];
    while (!table::is_binary_operator(copy)) {
        if (++pos == len)
            throw UnexpectedEndOfExpression(pos);
        copy += str[pos];
    }
    start = pos + 1;
    return table::get_binary_operator(copy);
}

shared_ptr<Operand> create_tree_from_parser_lists(list<shared_ptr<Operand>> operands, list<shared_ptr<BinaryOperator>> operators)
{
    if (operands.size() == 1)
        return *operands.begin();
    shared_ptr<Expression> res = make_shared<Expression>();
    list<shared_ptr<Operand>>::iterator operand = operands.begin();
    list<shared_ptr<Operand>>::iterator left_operand = operand++;
    list<shared_ptr<BinaryOperator>>::iterator op_it = operators.begin();
    list<shared_ptr<BinaryOperator>>::iterator hang_point = op_it++;
    while (op_it != operators.end()) {
        if ((*hang_point)->order() <= (*op_it)->order()) {
            hang_point = op_it;
            left_operand = operand;
        }
        ++op_it;
        ++operand;
    }
    res->set_root(*hang_point);
    (*hang_point)->set_left(create_tree_from_parser_lists(
        {operands.begin(), ++left_operand},
        {operators.begin(), hang_point}));
    (*hang_point)->set_right(create_tree_from_parser_lists(
        {left_operand, operands.end()},
        {++hang_point, operators.end()}));
    return res;
}

shared_ptr<Operand> parse_infix_expression(const string &str, size_t start)
{
    const size_t len = str.length();
    if (len == 0)
        return shared_ptr<Operand>(new Constant(0));
    /*
     * At first, there will definetly be either a constant or an unary operator
     * with its own operand. If not, there's no parsable expression anyway,
     * so exception fallback from parse_operand is acceptable.
     */
    size_t pos = start;
    shared_ptr<Operand> tmpoperand;
    shared_ptr<BinaryOperator> tmpoperator;
    list<shared_ptr<BinaryOperator>> operators;
    list<shared_ptr<Operand>> operands;
    do {
        pos = skip_spaces(str, pos);
        tmpoperand = parse_operand(str, pos);
        operands.push_back(tmpoperand);
        pos = skip_spaces(str, pos);
        if (pos < len) {
            tmpoperator = parse_operator(str, pos);
            operators.push_back(tmpoperator);
        } else {
            break;
        }
    } while (true);

    shared_ptr<Operand> res;
    if (operands.size() == 1)
        res = *operands.begin();
    else
        res = create_tree_from_parser_lists(operands, operators);
    return res;
}

shared_ptr<Operand> parse_prefix_operand(const string &str, size_t &start)
{
    const size_t len = str.length();
    if (start >= len)
        throw UnexpectedEndOfExpression(start);
    size_t pos = skip_spaces(str, start);
    if (pos == len)
        throw OperandExpectationUnsatisfied(pos);
    
    size_t backup_pos = pos;
    shared_ptr<Operand> res;
    if (table::is_starting_digit(str[pos])) {
        res = parse_value(str, pos);
    } else {
        try {
            res = parse_constant(str, pos);
        } catch (const UnexpectedEndOfExpression&) {
            throw OperandExpectationUnsatisfied(backup_pos);
        }
    }
    return res;
}

shared_ptr<Operand> parse_prefix_expression(const string &str, size_t start)
{

    const size_t len = str.length();
    if (len == 0)
        return shared_ptr<Operand>(new Constant(0));
    
    size_t pos = skip_spaces(str, start);
    shared_ptr<Operand> res;
    shared_ptr<Operator> tmpoperator;
    try {
        shared_ptr<BinaryOperator> tmp = parse_operator(str, start);
        tmp->set_left(parse_prefix_expression(str, pos));
        tmp->set_right(parse_prefix_expression(str, pos));
        shared_ptr<Expression> exp = make_shared<Expression>();
        exp->set_root(tmp);
        res = exp;
    } catch (const ParserError &) {
        try {
            shared_ptr<UnaryOperator> tmp = parse_unary(str, pos);
            tmp->set_operand(parse_prefix_expression(str, pos));
            shared_ptr<Expression> exp = make_shared<Expression>();
            exp->set_root(tmp);
            res = exp;
        } catch (const ParserError &) {
            res = parse_prefix_operand(str, pos);
        }
    }
    return res;
}

}   // namespace parsing
