#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "sorts.hh"

using namespace std;

vector<int> readData()
{
    ifstream in("data.txt");
    size_t n;
    in >> n;
    vector<int> data(n);
    for (size_t i = 0; i < n; ++i)
        in >> data[i];
    return data;
}

void measureInsertionSort(vector<int> &data)
{
    size_t step = 100;
    vector<int> slice;
    ofstream out("insertion.txt");
    for (size_t size = step; size <= 10000; size += step) {
        copy(
            data.begin(),
            data.begin() + size,
            back_inserter(slice)
        );
        chrono::steady_clock timer;
        chrono::time_point<chrono::steady_clock> start = timer.now();
        insertionSort(slice.begin(), slice.end());
        chrono::time_point<chrono::steady_clock> finish = timer.now();
        out << size << " entries sorted in "
            << (finish - start) / chrono::milliseconds(1) << "ms" << endl;
        slice.clear();
    }
}

void measureSelectionSort(vector<int> &data)
{
    size_t step = 100;
    vector<int> slice;
    ofstream out("selection.txt");
    for (size_t size = step; size <= 10000; size += step) {
        copy(
            data.begin(),
            data.begin() + size,
            back_inserter(slice)
        );
        chrono::steady_clock timer;
        chrono::time_point<chrono::steady_clock> start = timer.now();
        selectionSort(slice.begin(), slice.end());
        chrono::time_point<chrono::steady_clock> finish = timer.now();
        out << size << " entries sorted in "
            << (finish - start) / chrono::milliseconds(1) << "ms" << endl;
        slice.clear();
    }
}

void measureBubbleSort(vector<int> &data)
{
    size_t step = 100;
    vector<int> slice;
    ofstream out("bubble.txt");
    for (size_t size = step; size <= 10000; size += step) {
        copy(
            data.begin(),
            data.begin() + size,
            back_inserter(slice)
        );
        chrono::steady_clock timer;
        chrono::time_point<chrono::steady_clock> start = timer.now();
        bubbleSort(slice.begin(), slice.end());
        chrono::time_point<chrono::steady_clock> finish = timer.now();
        out << size << " entries sorted in "
            << (finish - start) / chrono::milliseconds(1) << "ms" << endl;
        slice.clear();
    }
}

void measureMergeSort(vector<int> &data)
{
    size_t step = 10000;
    vector<int> slice;
    ofstream out("merge.txt");
    for (size_t size = step; size <= data.size(); size += step) {
        copy(
            data.begin(),
            data.begin() + size,
            back_inserter(slice)
        );
        chrono::steady_clock timer;
        chrono::time_point<chrono::steady_clock> start = timer.now();
        mergeSort(slice.begin(), slice.end());
        chrono::time_point<chrono::steady_clock> finish = timer.now();
        out << size << " entries sorted in "
            << (finish - start) / chrono::milliseconds(1) << "ms" << endl;
        slice.clear();
    }
}

void measureShellSort(vector<int> &data)
{
    size_t step = 100;
    vector<int> slice;
    ofstream out("shell.txt");
    for (size_t size = step; size <= 10000; size += step) {
        copy(
            data.begin(),
            data.begin() + size,
            back_inserter(slice)
        );
        chrono::steady_clock timer;
        chrono::time_point<chrono::steady_clock> start = timer.now();
        shellSort(slice.begin(), slice.end());
        chrono::time_point<chrono::steady_clock> finish = timer.now();
        out << size << " entries sorted in "
            << (finish - start) / chrono::milliseconds(1) << "ms" << endl;
        slice.clear();
    }
}

void measureQuickSort(vector<int> &data)
{
    size_t step = 10000;
    vector<int> slice;
    ofstream out("quick.txt");
    for (size_t size = step; size <= data.size(); size += step) {
        copy(
            data.begin(),
            data.begin() + size,
            back_inserter(slice)
        );
        chrono::steady_clock timer;
        chrono::time_point<chrono::steady_clock> start = timer.now();
        quickSort(slice.begin(), slice.end());
        chrono::time_point<chrono::steady_clock> finish = timer.now();
        out << size << " entries sorted in "
            << (finish - start) / chrono::milliseconds(1) << "ms" << endl;
        slice.clear();
    }
}

void measureSTLSort(vector<int> &data)
{
    size_t step = 10000;
    vector<int> slice;
    ofstream out("stl.txt");
    for (size_t size = step; size <= data.size(); size += step) {
        copy(
            data.begin(),
            data.begin() + size,
            back_inserter(slice)
        );
        chrono::steady_clock timer;
        chrono::time_point<chrono::steady_clock> start = timer.now();
        sort(slice.begin(), slice.end());
        chrono::time_point<chrono::steady_clock> finish = timer.now();
        out << size << " entries sorted in "
            << (finish - start) / chrono::milliseconds(1) << "ms" << endl;
        slice.clear();
    }
}

int main()
{
    vector<int> data = readData();
    measureInsertionSort(data);
    measureSelectionSort(data);
    measureBubbleSort(data);
    measureMergeSort(data);
    measureShellSort(data);
    measureSTLSort(data);
    measureQuickSort(data);
    return 0;
}
