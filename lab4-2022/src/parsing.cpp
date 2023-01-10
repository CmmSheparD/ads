#include "parsing.hh"

#include <cmath>
#include <list>
#include <string>
#include <utility>

#include "parsing-exceptions.hh"
#include "parsing-table.hh"

using table = ParsingTable;
using namespace std;

void init_table()
{
    table::register_operator("^", 1);
    table::register_operator("*", 2);
    table::register_operator("/", 2);
    table::register_operator("+", 3);
    table::register_operator("-", 3);
}

size_t skipSpaces(const string &str, size_t start)
{
    const size_t len = str.size();
    size_t pos = start;
    while (pos < len && isspace(str[pos]))
        ++pos;
    return pos;
}

Component parseValue(const string &str, size_t &start)
{
    size_t processed = 0;
    try {
        string buffer(str, start, str.size() - start);
        stod(buffer, &processed);
    } catch (out_of_range &) {
        throw TooBigNumber(start);
    }
    Component res(string(str, start, processed), 0, Component::kOperand);
    start += processed;
    return res;
}

Component parseConstant(const string &str, size_t &start)
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

list<Component> parseInfixOperand(const string &str, size_t &start)
{
    const size_t len = str.length();
    if (start >= len)
        throw UnexpectedEndOfExpression(start);
    size_t pos = skipSpaces(str, start);
    if (pos == len)
        throw OperandExpectationUnsatisfied(pos);

    list<Component> res;
    while (true) {
        size_t backup_pos = pos;
        if (str[pos] == '(') {
            res.push_back({"(", 0, Component::kSpecial});
            ++pos;
        } else if (str[pos] == ')') {
            res.push_back({")", 0, Component::kSpecial});
            ++pos;
        } else if (table::is_starting_digit(str[pos])) {
            res.push_back(parseValue(str, pos));
            break;
        } else {
            try {
                res.push_back(parseConstant(str, pos));
            } catch (const UnexpectedEndOfExpression&) {
                throw OperandExpectationUnsatisfied(backup_pos);
            }
            break;
        }
    }
    start = pos;
    return res;
}

list<Component> parseInfixOperator(const string &str, size_t &start)
{
    const size_t len = str.length();
    if (start >= len)
        throw UnexpectedEndOfExpression(len);
    size_t pos = skipSpaces(str, start);
    if (pos == len)
        throw OperatorExpectationUnsatisfied(start);

    list<Component> res;
    while (pos < len && str[pos] == ')') {
        res.push_back({")", 0, Component::kSpecial});
        ++pos;
    }
    if (pos >= len) {
        start = pos;
        return res;
    }
    pos = skipSpaces(str, pos);
    string copy;
    copy += str[pos];
    while (!table::is_operator(copy)) {
        if (++pos == len)
            throw UnexpectedEndOfExpression(pos);
        copy += str[pos];
    }
    start = pos + 1;
    res.push_back(table::get_operator(copy));
    return res;
}

list<pair<int, Component>> applyBrackets(const list<Component> &components)
{
    int counter = 0;
    int modifier = 0;
    list<pair<int, Component>> res;
    list<Component>::const_iterator it = components.cbegin();
    for (; it != components.end(); ++it) {
        if (it->str() == "(") {
            modifier -= 5;
            ++counter;
        } else if (it->str() == ")") {
            modifier += 5;
            --counter;
        } else {
            res.push_back({it->getPriority() + modifier, *it});
        }
        if (counter < 0) throw SyntaxError("Brackets error", 0);
    }
    if (counter != 0) throw SyntaxError("Brackets error", 0);
    return res;
}

void fillTreeFromList(list<pair<int, Component>> components,
                      BST<int, Component> &tree, size_t max_depth,
                      size_t depth = 0)
{
    if (components.size() == 1) {
        tree.insert(components.front().first, components.front().second);
        return;
    }
    list<pair<int, Component>>::iterator op = components.begin();
    list<pair<int, Component>>::iterator operand = op;
    list<pair<int, Component>>::iterator left = op++;
    list<pair<int, Component>>::iterator hang_point = op;
    while (op != components.end()) {
        if (op->second.getType() == Component::kOperator
            && hang_point->first <= op->first) {
            hang_point = op;
            left = operand;
        }
        ++op;
        ++operand;
    }
    tree.insert(hang_point->first, hang_point->second);
    int modifier = pow(2, max_depth - depth);
    for (auto i = components.begin(); i != hang_point; ++i) i->first -= modifier;
    fillTreeFromList({components.begin(), ++left}, tree, max_depth, depth + 1);
    for (auto i = ++hang_point; i != op; ++i) i->first += modifier;
    fillTreeFromList({hang_point, op}, tree, max_depth, depth + 1);
}

BST<int, Component> createTreeFromParserList(list<Component> components)
{
    list<pair<int, Component>> priority_list = applyBrackets(components);
    BST<int, Component> tree;
    fillTreeFromList(priority_list, tree, priority_list.size());
    return tree;
}

Expression parseInfixExpression(const string &str, size_t start)
{
    const size_t len = str.length();
    if (len == 0)
        throw invalid_argument("Empty string.");
    /*
     * At first, there will definetly be either a constant or an unary operator
     * with its own operand. If not, there's no parsable expression anyway,
     * so exception fallback from parseInfixOperand is acceptable.
     */
    size_t pos = start;
    list<Component> ops;
    list<Component> components;
    do {
        pos = skipSpaces(str, pos);
        ops = parseInfixOperand(str, pos);
        for (Component c : ops)
            components.push_back(c);
        pos = skipSpaces(str, pos);
        if (pos < len) {
            ops = parseInfixOperator(str, pos);
            for (Component c : ops)
                components.push_back(c);
        } else {
            break;
        }
    } while (pos < len);

    return Expression(createTreeFromParserList(components));
}

Component parsePrefixOperand(const string &str, size_t &start)
{
    const size_t len = str.length();
    if (start >= len)
        throw UnexpectedEndOfExpression(start);
    size_t pos = skipSpaces(str, start);
    if (pos == len)
        throw OperandExpectationUnsatisfied(pos);
    
    size_t backup_pos = pos;
    Component res;
    if (table::is_starting_digit(str[pos])) {
        res = parseValue(str, pos);
    } else {
        try {
            res = parseConstant(str, pos);
        } catch (const UnexpectedEndOfExpression&) {
            throw OperandExpectationUnsatisfied(backup_pos);
        }
    }
    start = pos;
    return res;
}

list<Component> parsePrefixExpressionToList(const string &str, size_t &start)
{
    const size_t len = str.length();
    size_t pos = skipSpaces(str, start);
    if (len - pos == 0)
        throw OperandExpectationUnsatisfied(pos);
    list<Component> res;
    Component tmpoperator;
    size_t backup_pos = pos;
    try {
        list<Component> tmp = parseInfixOperator(str, pos);
        for (Component c : tmp) res.push_back(c);
        tmp = parsePrefixExpressionToList(str, pos);
        for (Component c : tmp) res.push_back(c);
        tmp = parsePrefixExpressionToList(str, pos);
        for (Component c : tmp) res.push_back(c);
    } catch (const ParserError &) {
        pos = backup_pos;
        res.push_back(parsePrefixOperand(str, pos));
    }
    start = pos;
    return res;
}

void fillTreeFromPrefixList(list<Component>::iterator &first,
                            BST<int, Component> &tree, size_t max_depth,
                            size_t depth = 0, int mod = 0)
{
    tree.insert(first->getPriority() + mod, *first);
    if (first->getType() != Component::kOperator) {
        ++first;
        return;
    }
    ++first;
    fillTreeFromPrefixList(first, tree, max_depth, depth + 1, mod - pow(2, max_depth - depth));
    fillTreeFromPrefixList(first, tree, max_depth, depth + 1, mod + pow(2, max_depth - depth));
}

BST<int, Component> createTreeFromPrefixList(list<Component> &components)
{
    BST<int, Component> tree;
    list<Component>::iterator it = components.begin();
    fillTreeFromPrefixList(it, tree, components.size());
    return tree;
}

Expression parsePrefixExpression(const string &str)
{
    size_t start = 0;
    list<Component> tmp = parsePrefixExpressionToList(str, start);
    BST<int, Component> tree = createTreeFromPrefixList(tmp);
    return Expression(tree);
}


Component parsePostfixOperator(const string &rev, size_t &start)
{
    const size_t len = rev.length();
    if (start >= len)
        throw UnexpectedEndOfExpression(len);
    size_t pos = skipSpaces(rev, start);
    if (pos >= len)
        throw OperatorExpectationUnsatisfied(start);

    string copy;
    copy += rev[pos];
    while (!table::is_operator(copy)) {
        if (++pos >= len)
            throw UnexpectedEndOfExpression(pos);
        copy.insert(copy.begin(), rev[pos]);
    }
    start = pos + 1;
    return table::get_operator(copy);
}

Component parsePostfixValue(const string &rev, size_t &start)
{
    const size_t len = rev.length();
    size_t processed = 0;
    try {
        string buffer;
        char c;
        while (start + processed < len) {
            c = rev[start + processed];
            if (!isdigit(c) && c != '.')
                break;
            buffer.insert(buffer.begin(), c);
            ++processed;
        }
        stod(buffer, &processed);
    } catch (out_of_range &) {
        throw TooBigNumber(start);
    }
    Component res(string(rev, start - processed + 1, processed), 0, Component::kOperand);
    start += processed;
    return res;
}

Component parsePostfixConstant(const string &rev, size_t &start)
{
    const size_t len = rev.length();
    size_t pos = start;
    string copy;
    copy += rev[pos];
    while (!table::is_constant(copy)) {
        if (++pos >= len)
            throw UnexpectedEndOfExpression(pos);
        copy.insert(copy.begin(), rev[pos]);
    }
    start = pos + 1;
    return table::get_constant(copy);
}

Component parsePostfixOperand(const string &str, size_t &start)
{
    const size_t len = str.length();
    if (start >= len)
        throw UnexpectedEndOfExpression(start);
    size_t pos = skipSpaces(str, start);
    if (pos >= len)
        throw OperandExpectationUnsatisfied(pos);
    
    size_t backup_pos = pos;
    Component res;
    try {
        res = parsePostfixConstant(str, pos);
    } catch (const ParserError &) {
        pos = backup_pos;
        try {
            res = parsePostfixValue(str, pos);
        } catch (const UnexpectedEndOfExpression&) {
            throw OperandExpectationUnsatisfied(backup_pos);
        }
    }
    start = pos;
    return res;
}

list<Component> parsePostfixExpressionToList(const std::string &rev, size_t &start)
{
    const size_t len = rev.length();
    size_t pos = skipSpaces(rev, start);
    if (len - pos == 0)
        throw OperandExpectationUnsatisfied(pos);
    list<Component> res;
    size_t backup_pos = pos;
    try {
        res.push_back(parsePostfixOperator(rev, pos));
        list<Component> tmp = parsePostfixExpressionToList(rev, pos);
        for (Component c : tmp) res.push_back(c);
        tmp = parsePostfixExpressionToList(rev, pos);
        for (Component c : tmp) res.push_back(c);
    } catch (const ParserError &) {
        pos = backup_pos;
        res.push_back(parsePostfixOperand(rev, pos));
    }
    start = pos;
    return res;
}

void fillTreeFromPostfixList(list<Component>::iterator &first,
                             BST<int, Component> &tree, size_t max_depth,
                             size_t depth = 0, int mod = 0)
{
    tree.insert(first->getPriority() + mod, *first);
    if (first->getType() != Component::kOperator) {
        ++first;
        return;
    }
    ++first;
    fillTreeFromPostfixList(first, tree, max_depth, depth + 1, mod + pow(2, max_depth - depth));
    fillTreeFromPostfixList(first, tree, max_depth, depth + 1, mod - pow(2, max_depth - depth));
}

BST<int, Component> createTreeFromPostfixList(list<Component> &components)
{
    BST<int, Component> tree;
    list<Component>::iterator it = components.begin();
    fillTreeFromPostfixList(it, tree, components.size());
    return tree;
}

Expression parsePostfixExpression(const std::string &str)
{
    size_t start = 0;
    string rev(str.rbegin(), str.rend());
    list<Component> tmp = parsePostfixExpressionToList(rev, start);
    BST<int, Component> tree = createTreeFromPostfixList(tmp);
    return Expression(tree);
}
