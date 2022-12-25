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

int main()
{
    init_table();
    cout << std::defaultfloat;
    string buffer;
    shared_ptr<Operand> res;
    do {
        cout << "> ";
        getline(cin, buffer);
        if (buffer.empty())
            break;
        try {
            res = parse_prefix_expression(buffer);
        } catch (const ParserError &e) {
            cout << buffer << endl;
            for (size_t i = 0; i < e.position; ++i)
                cout << ' ';
            cout << '^' << endl;
            cout << e.what() << endl;
            continue;
        }
        cout << res->str() << endl;
        cout << res->evaluate() << endl;
    } while (true);
    return 0;
}
