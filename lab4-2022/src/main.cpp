#include <iostream>
#include <string>
#include <vector>

#include "bst.hh"
#include "parsing.hh"

using namespace std;


int main()
{
    string s = "* - 2 3 4";
    init_table();
    Expression e = parsePrefixExpression(s);
    cout << e.asPostfix() << endl;
    cout << e.asInfix() << endl;
    cout << e.asPrefix() << endl;
    return 0;
}
