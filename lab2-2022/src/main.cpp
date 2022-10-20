#include <algorithm>
#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

#include "generate.hh"
#include "snail.hh"
#include "solve.hh"

using namespace std;

vector<Snail> readSnails()
{
    ifstream in("snails.txt");
    if (!in)
        throw runtime_error("No snails file found.");
    size_t n;
    in >> n;
    vector<Snail> snails;
    try {
        while (n--) {
            double x, y;
            in >> x >> y;
            snails.push_back({ x, y });
        }
    } catch (exception &) {
        throw runtime_error("File format error.");
    }
    in.close();
    return snails;
}

void measure(vector<Snail> &snails, size_t n)
{
    vector<Snail> slice;
    copy(
        snails.cbegin(),
        snails.cbegin() + n,
        back_inserter(slice)
    );
    chrono::steady_clock timer;
    chrono::time_point<chrono::steady_clock> start = timer.now();
    Solution s = findClosestDistance(slice);
    chrono::time_point<chrono::steady_clock> finish = timer.now();
    if (s.status == Solution::kOk)
        cout << "Answer: " << s.answer << endl;
    else
        cout << "Not enough snails." << endl;
    cout << "Finished in " << (finish - start) / chrono::milliseconds(1) << "ms" << endl;
}

int main()
{
	vector<Snail> snails;
    try {
        snails = readSnails();
        cout << "Read " << snails.size() << " snails." << endl;
    } catch (runtime_error &) {
        cout << "Snails file format error." << endl;
        size_t n = 0;
        cout << "How many to generate: ";
        do {
            cin >> n;
            if (n == 0) 
                cout << "Try again: ";
        } while (n == 0);
        snails = generate(n);
        cout << "Generated " << n << " snails." << endl;
    }
    size_t step = 1000;
    for (size_t size = step; size <= snails.size(); size += step) {
        if (size == step * 10)
            step = size;
        cout << "For selection of " << size << " snails:" << endl;
        measure(snails, size);
        cout << endl;
    }
	return 0;
}
