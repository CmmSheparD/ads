#include "snail.hh"

#include <cmath>

using namespace std;

double Snail::distanceTo(const Snail &other) const
{
    return abs(sqrt(pow(x - other.x, 2) + pow(y - other.y, 2)));
}

bool Snail::operator==(const Snail &other) const
{
    return x == other.x && y == other.y;
}
