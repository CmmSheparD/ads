#include "solve.hh"

#include <algorithm>
#include <vector>

using namespace std;

struct Slice {
    Slice(vector<const Snail *>::iterator l, vector<const Snail *>::iterator r)
        : left(l), right(r), len(r - l)
    {}

    vector<Slice> split()
    {
        return {
            {left, right - len / 2},
            {right - len / 2, right}
        };
    }

    vector<const Snail *>::iterator left;
    vector<const Snail *>::iterator right;
    ptrdiff_t len;
};


Solution baseCase(Slice &slice)
{
    vector<const Snail *>::iterator right = slice.left + 1;
    Solution res = {
        kOk,
        (*slice.left)->distanceTo(**right)
    };
    if ((*slice.left)->y > (*right)->y) {
        const Snail *t = *right;
        *right = *slice.left;
        *slice.left = t;
    }
    return res;
}

Solution findClosestDistanceInSquare(Slice &slice)
{
    if (slice.len < 2)
        return Solution();
    if (slice.len < 3)
        return baseCase(slice);
    vector<Slice> split = slice.split();
    double mid_x = (*split[1].left)->x;

    Solution res = findClosestDistanceInSquare(split[0]);
    Solution tmp_res = findClosestDistanceInSquare(split[1]);
    if (tmp_res.status == kOk
        && (res.status != kOk
        || tmp_res.answer < res.answer))
        res = tmp_res;
    inplace_merge(
        slice.left,
        split[1].left,
        slice.right,
        [](const Snail *a, const Snail *b)
        {
            return a->y < b->y;
        }
    );
    for (auto i = slice.left; i != slice.right; ++i) {
        if (abs((*i)->x - mid_x) <= res.answer) {
            for (vector<const Snail *>::iterator j = i + 1; 
                j != slice.right
                && (*j)->y - (*i)->y <= res.answer;
                ++j) {

                double ans = (*i)->distanceTo(**j);
                if (ans < res.answer)
                    res.answer = ans;
            }
        }
    }
    return res;
}

Solution findClosestDistance(vector<Snail> snails)
{
    if (snails.size() < 2)
        return Solution();
    vector<const Snail *> sorted(snails.size(), nullptr);
    transform(
        snails.cbegin(),
        snails.cend(),
        sorted.begin(),
        [](const Snail &s) { return &s; }
    );
    sort(
        sorted.begin(),
        sorted.end(),
        [](const Snail *first, const Snail *second)
        {
            return first->x < second->x
                   || first->x == second->x && first->y < second->y;
        }
    );
    Slice slice = {sorted.begin(), sorted.end()};
    Solution res = findClosestDistanceInSquare(slice);
    return res;
}
