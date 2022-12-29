#include "parsing.hh"

#include <iostream>
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
    table::register_constant("pi");
    table::register_constant("e");

    table::register_operator("^", 1);
    table::register_operator("*", 2);
    table::register_operator("/", 2);
    table::register_operator("+", 3);
    table::register_operator("-", 3);
}

void out(string s, size_t from)
{
    for (size_t i = from; i < s.length(); ++i)
        cout << s[i];
    cout << endl;
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
    double val;
    try {
        string buffer(str, start, str.size() - start);
        val = stod(buffer, &processed);
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
            --modifier;
            ++counter;
        } else if (it->str() == ")") {
            ++modifier;
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
    out(str, pos);
    if (len - pos == 0)
        throw OperandExpectationUnsatisfied(pos);
    list<Component> res;
    Component tmpoperator;
    size_t backup_pos = pos;
    try {
        list<Component> tmp = parseInfixOperator(str, pos);
        for (Component c : tmp) res.push_back(c);
        cout << "Parsed operator" << endl;
        tmp = parsePrefixExpressionToList(str, pos);
        for (Component c : tmp) res.push_back(c);
        cout << "Parsed first operand" << endl;
        tmp = parsePrefixExpressionToList(str, pos);
        for (Component c : tmp) res.push_back(c);
        cout << "Parsed second operand" << endl;
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
    cout << "Inserted local root " << first->str() << endl;
    if (first->getType() != Component::kOperator) {
        ++first;
        return;
    }
    ++first;
    fillTreeFromPrefixList(first, tree, max_depth, depth + 1, -pow(2, max_depth - depth));
    fillTreeFromPrefixList(first, tree, max_depth, depth + 1, pow(2, max_depth - depth));
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


// shared_ptr<UnaryOperator> parse_postfix_unary(const string &rev, size_t &start)
// {
//     const size_t len = rev.length();
//     size_t pos = skipSpaces(rev, start);
//     string copy;
//     copy += rev[pos];
//     while (!table::is_unary_operator(copy)) {
//         if (++pos >= len)
//             throw UnexpectedEndOfExpression(pos);
//         copy.insert(copy.begin(), rev[pos]);
//     }
//     start = pos + 1;
//     return table::get_unary_operator(copy);
// }

// shared_ptr<BinaryOperator> parse_postfix_operator(const string &rev, size_t &start)
// {
//     const size_t len = rev.length();
//     if (start >= len)
//         throw UnexpectedEndOfExpression(len);
//     size_t pos = skipSpaces(rev, start);
//     if (pos >= len)
//         throw OperatorExpectationUnsatisfied(start);

//     string copy;
//     copy += rev[pos];
//     while (!table::is_binary_operator(copy)) {
//         if (++pos >= len)
//             throw UnexpectedEndOfExpression(pos);
//         copy.insert(copy.begin(), rev[pos]);
//     }
//     start = pos + 1;
//     return table::get_binary_operator(copy);
// }

// shared_ptr<Constant> parse_postfix_value(const string &rev, size_t &start)
// {
//     const size_t len = rev.length();
//     size_t processed = 0;
//     double val;
//     try {
//         string buffer;
//         char c;
//         while (start + processed < len) {
//             c = rev[start + processed];
//             if (!isdigit(c) && c != '.')
//                 break;
//             buffer.insert(buffer.begin(), c);
//             ++processed;
//         }
//         val = stod(buffer, &processed);
//     } catch (out_of_range &) {
//         throw TooBigNumber(start);
//     }
//     shared_ptr<Constant> res = make_shared<Constant>(
//         val,
//         string(rev, start - processed + 1, processed)
//     );
//     start += processed;
//     return res;
// }

// shared_ptr<Constant> parse_postfix_constant(const string &rev, size_t &start)
// {
//     const size_t len = rev.length();
//     size_t pos = start;
//     string copy;
//     copy += rev[pos];
//     while (!table::is_constant(copy)) {
//         if (++pos >= len)
//             throw UnexpectedEndOfExpression(pos);
//         copy.insert(copy.begin(), rev[pos]);
//     }
//     start = pos + 1;
//     return table::get_constant(copy);
// }

// shared_ptr<Operand> parse_postfix_operand(const string &str, size_t &start)
// {
//     const size_t len = str.length();
//     if (start >= len)
//         throw UnexpectedEndOfExpression(start);
//     size_t pos = skipSpaces(str, start);
//     if (pos >= len)
//         throw OperandExpectationUnsatisfied(pos);
    
//     size_t backup_pos = pos;
//     shared_ptr<Operand> res;
//     try {
//         res = parse_postfix_constant(str, pos);
//     } catch (const ParserError &) {
//         pos = backup_pos;
//         try {
//             res = parse_postfix_value(str, pos);
//         } catch (const UnexpectedEndOfExpression&) {
//             throw OperandExpectationUnsatisfied(backup_pos);
//         }
//     }
//     start = pos;
//     return res;
// }

// std::shared_ptr<calculation::Operand> parse_postfix_expression_worker(const std::string &rev, size_t &start)
// {
//     const size_t len = rev.length();
//     size_t pos = skipSpaces(rev, start);
//     if (len - pos == 0)
//         throw OperandExpectationUnsatisfied(start);
//     shared_ptr<Operand> res;
//     shared_ptr<Operator> tmpoperator;
//     size_t backup_pos = pos;
//     try {
//         shared_ptr<BinaryOperator> tmp = parse_postfix_operator(rev, pos);
//         tmp->set_right(parse_postfix_expression_worker(rev, pos));
//         tmp->set_left(parse_postfix_expression_worker(rev, pos));
//         shared_ptr<Expression> exp = make_shared<Expression>();
//         exp->set_root(tmp);
//         res = exp;
//     } catch (const ParserError &) {
//         pos = backup_pos;
//         try {
//             shared_ptr<UnaryOperator> tmp = parse_postfix_unary(rev, pos);
//             tmp->set_operand(parse_postfix_expression_worker(rev, pos));
//             shared_ptr<Expression> exp = make_shared<Expression>();
//             exp->set_root(tmp);
//             res = exp;
//         } catch (const ParserError &) {
//             pos = backup_pos;
//             res = parse_postfix_operand(rev, pos);
//         }
//     }
//     start = pos;
//     return res;
// }

// std::shared_ptr<calculation::Operand> parse_postfix_expression(const std::string &str)
// {
//     size_t start = 0;
//     string rev(str.rbegin(), str.rend());
//     return parse_postfix_expression_worker(rev, start);
// }
