#pragma once
#ifndef PARSING_HH
#define PARSING_HH

#include <string>

#include "expression.hh"

void init_table();

/*
 * Returns an expression, evaluation of which will calculate the expression
 * stored in the string.
 */
Expression parseInfixExpression(const std::string &str, size_t start = 0);

Expression parsePrefixExpression(const std::string &str);

Expression parsePostfixExpression(const std::string &str);

#endif  // PARSING_HH
