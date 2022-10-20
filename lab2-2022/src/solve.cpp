#include "solve.hh"

#include <algorithm>
#include <iostream>
#include <list>

using namespace std;

struct Square {
    Square(const vector<const Snail *> snails, double w, double e, double n, double s)
        : west_most(e), east_most(w), north_most(s), south_most(n)
    {
        this->snails.reserve(snails.size());
        for (const Snail *s : snails) {
            if (west_most > s->x) west_most = s->x;
            if (east_most < s->x) east_most = s->x;
            if (north_most > s->y) north_most = s->y;
            if (south_most < s->y) south_most = s->y;
            this->snails.push_back(s);
        }
    }

    list<Square> split()
    {
        return {
            //  **..
            //  **..
            //  ....
            //  ....
            filter(west_most, (west_most + east_most) / 2,
                   north_most, (north_most + south_most) / 2),
            //  ..**
            //  ..**
            //  ....
            //  ....
            filter((west_most + east_most) / 2, east_most,
                   north_most, (north_most + south_most) / 2),
            //  ....
            //  ....
            //  **..
            //  **..
            filter(west_most, (west_most + east_most) / 2,
                   (north_most + south_most) / 2, south_most),
            //  ....
            //  ....
            //  ..**
            //  ..**
            filter((west_most + east_most) / 2, east_most,
                   (north_most + south_most) / 2, south_most),
            //  .**.
            //  .**.
            //  ....
            //  ....
            filter((3 * west_most + east_most) / 4, (west_most + 3 * east_most) / 4,
                   north_most, (north_most + south_most) / 2),
            //  ....
            //  ....
            //  .**.
            //  .**.
            filter((3 * west_most + east_most) / 4, (west_most + 3 * east_most) / 4,
                   (north_most + south_most) / 2, south_most),
            //  ....
            //  **..
            //  **..
            //  ....
            filter(west_most, (west_most + east_most) / 2,
                   (3 * north_most + south_most) / 4, (north_most + 3 * south_most) / 4),
            //  ....
            //  ..**
            //  ..**
            //  ....
            filter((west_most + east_most) / 2, east_most,
                   (3 * north_most + south_most) / 4, (north_most + 3 * south_most) / 4),
            //  ....
            //  .**.
            //  .**.
            //  ....
            filter((3 * west_most + east_most) / 4, (west_most + 3 * east_most) / 4,
                   (3 * north_most + south_most) / 4, (north_most + 3 * south_most) / 4),
        };
    }

    Square filter(double west_most, double east_most, double north_most, double south_most)
    {
        
        vector<const Snail*> slice;
        copy_if(
            snails.cbegin(),
            snails.cend(),
            back_inserter(slice),
            [&](const Snail *snail)
            {
                return west_most <= snail->x && snail->x <= east_most
                    && north_most <= snail->y && snail->y <= south_most;
            }
        );
        return { slice, west_most, east_most, north_most, south_most };
    }

    vector<const Snail *> snails;
    double west_most;
    double east_most;
    double north_most;
    double south_most;
};

Solution baseCase(Square &square)
{
    const size_t len = square.snails.size();
    double min = -1;
    double tmp;
    for (size_t i = 0; i < len - 1; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            tmp = square.snails[i]->distanceTo(*square.snails[j]);
            if (min == -1 || tmp < min)
                min = tmp;
        }
    }
    return { Solution::kOk, min };
}

Solution findClosestDistanceInSquare(Square square)
{
    if (square.snails.size() < 2)
        return { Solution::kNotEnough, -1 };
    if (square.snails.size() < 5)
        return baseCase(square);
    double min = -1;
    Solution tmp_res;
    for (Square &s : square.split()) {
        tmp_res = findClosestDistanceInSquare(s);
        if (tmp_res.status == Solution::kOk
            && (min == -1 || tmp_res.answer < min))
            min = tmp_res.answer;
    }
    return { Solution::kOk, min };
}

Solution findClosestDistance(vector<Snail> snails)
{
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
            return first->x < second->x;
        }
    );
    Square base_square = { sorted, 0, 1000, 0, 1000 };
    return findClosestDistanceInSquare(base_square);
}
