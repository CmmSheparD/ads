#include <iostream>
#include <string>
#include <vector>

#include "bst.hh"

using namespace std;


int main()
{
    BST<int, int> a;
    for (int i = 0; i < 5; ++i)
        a.insert(i, i);
    for (int i = -1; i > -5; --i)
        a.insert(i, i);
    a.insert(4, 1);
    a.insert(2, 2);
    a.insert(1, 4);
    a.insert(3, 8);
    a.insert(7, 5);
    auto it = a.breadthBegin();
    while (it) {
        cout << it.key() << ' ' << *it << endl;
        ++it;
    }
    auto r = a.visualize();
    for (auto s : r) cout << s << endl;
    return 0;
}
