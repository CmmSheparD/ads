#pragma once
#ifndef PARSING_HH
#define PARSING_HH

#include <string>

#include "calculation-tree.hh"

namespace parsing {

void init_table();

/*
 * Returns an expression, evaluation of which will calculate the expression
 * stored in the string.
 */
std::shared_ptr<calculation::Operand> parse_infix_expression(const std::string &str, size_t start = 0);

std::shared_ptr<calculation::Operand> parse_prefix_expression(const std::string &str);

std::shared_ptr<calculation::Operand> parse_postfix_expression(const std::string &str);

}   // namespace parsing

#endif  // PARSING_HH
