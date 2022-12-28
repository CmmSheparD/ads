#pragma once
#ifndef PARSING_TABLE_HH
#define PARSING_TABLE_HH

#include <cctype>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>

#include "component.hh"

namespace parsing {

/*
 * A static table that holds entries on how to decode text into math
 */
class ParsingTable {
public:
    class InvalidNameError;
    class NameSearchError;

    ParsingTable() = delete;
    ParsingTable(const ParsingTable &) = delete;
    ParsingTable(ParsingTable &&) = delete;

    static bool is_valid_name(const std::string &name);
    static bool is_starting_digit(char c) { return std::isdigit(c) || c == '-'; }
    static bool is_digit(char c) { return std::isdigit(c) || c == '.'; }

    static void register_constant(const std::string &name);
    static void register_operator(const std::string &name, int priority);

    static bool is_constant(const std::string &name);
    static bool is_operator(const std::string &name);

    static Component get_constant(const std::string &name);
    static Component get_operator(const std::string &name);
private:
    ~ParsingTable() = default;

    static std::list<Component> constants_;
    static std::list<Component> operators_;
};


class ParsingTable::InvalidNameError : public std::invalid_argument {
public:
    InvalidNameError(const std::string &name)
        : invalid_argument("Invalid name \"" + name + "\".")
    {}
};


class ParsingTable::NameSearchError : public std::invalid_argument {
public:
    NameSearchError(const std::string &name)
        : invalid_argument("No such name found \"" + name + "\".")
    {}
};

}   // namespace parsing

#endif  // PARSING_TABLE_HH
