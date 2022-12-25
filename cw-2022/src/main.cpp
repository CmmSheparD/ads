#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include "calculation-tree.hh"
#include "parsing.hh"
#include "parsing-exceptions.hh"

using namespace calculation;
using namespace parsing;
using namespace std;

enum Mode { kInfix, kPrefix, kPostfix };

int main()
{
    init_table();
    cout << std::defaultfloat;
    string buffer;
    shared_ptr<Operand> res;
    Mode mode = kInfix;
    do {
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
        if (buffer.empty())
            break;
        if (buffer[0] == ':') {
            if (buffer.substr(1) == "infix")
                mode = kInfix;
            else if (buffer.substr(1) == "prefix")
                mode = kPrefix;
            else if (buffer.substr(1) == "postfix")
                mode = kPostfix;
            else
                cout << "Unknown mode \"" << buffer.substr(1) << '"' << endl;
        } else {
            try {
                switch (mode) {
                case kInfix:
                    res = parse_infix_expression(buffer);
                    break;
                case kPrefix:
                    res = parse_prefix_expression(buffer);
                    break;
                case kPostfix:
                    res = parse_postfix_expression(buffer);
                    break;
                }
            } catch (const ParserError &e) {
                cout << "Invalid expression." << endl;
                continue;
            }
            cout << res->evaluate() << endl;
        }
    } while (true);
    return 0;
}
