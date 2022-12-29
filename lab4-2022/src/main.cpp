#include <iostream>
#include <string>
#include <vector>

#include "bst.hh"
#include "parsing-exceptions.hh"
#include "parsing.hh"

enum Mode { kInfix, kPrefix, kPostfix, kUnset };

using namespace std;

std::ostream &operator<<(std::ostream &os, const Component &c)
{
    os << c.str();
    return os;
}

int main()
{
    string s = "2 3 - 4 *";
    init_table();
    cout << std::defaultfloat;
    string buffer;
    Mode mode = kUnset;
    Expression res;
    do {
        cout << "Enter mode: ";
        getline(cin, buffer);
        if (buffer == "infix")
            mode = kInfix;
        else if (buffer == "prefix")
            mode = kPrefix;
        else if (buffer == "postfix")
            mode = kPostfix;
        else
            cout << "Unknown mode \"" << buffer << '"' << endl;
    } while (mode == kUnset);

    switch (mode) {
    case kInfix:
        cout << "infix ";
        break;
    case kPrefix:
        cout << "prefix ";
        break;
    case kPostfix:
        cout << "postfix ";
        break;
    }
    cout << "> ";
    getline(cin, buffer);
    if (!buffer.empty()) {
        try {
            switch (mode) {
            case kInfix:
                res = parseInfixExpression(buffer);
                break;
            case kPrefix:
                res = parsePrefixExpression(buffer);
                break;
            case kPostfix:
                res = parsePostfixExpression(buffer);
                break;
            }
        } catch (const ParserError &e) {
            cout << "Invalid expression." << endl;
        }
        cout << "Prefix form: " << res.asPrefix() << endl;
        cout << "Infix form: " << res.asInfix() << endl;
        cout << "Postfix form: " << res.asPostfix() << endl;
        auto a = res.visualize();
        for (auto l : a)
            cout << l << endl;
    }
    return 0;
}
