#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <stack>

#include "generate.hh"
#include "snail.hh"

using namespace std;

int main()
{
    size_t set_number = 0;
    cout << "Set number: ";
    do {
        cin >> set_number;
        if (set_number == 0) 
            cout << "Try again: ";
    } while (set_number == 0);

    size_t n = 0;
    cout << "How many to generate: ";
    do {
        cin >> n;
        if (n == 0) 
            cout << "Try again: ";
    } while (n == 0);

    vector<Snail> snails = generate(n);

    stringstream filename;
    stack<size_t> num_stack;
    while (n >= 1000) {
        num_stack.push(n % 1000);
        n /= 1000;
    }
    filename << n;
    while (!num_stack.empty()) {
        filename << '_';
        if (num_stack.top() == 0)
            filename << "000";
        else
            filename << num_stack.top();
        n = n * 1000 + num_stack.top();
        num_stack.pop();
    }
    filename << "-snails-set-" << set_number << ".txt";

    ofstream out(filename.str());
    out << n << endl;
    for (Snail &s : snails)
        out << s.x << ' ' << s.y << endl;
    out.close();
    return 0;
}
