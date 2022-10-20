#pragma once
#ifndef SOLVE_HH
#define SOLVE_HH

#include <vector>

#include "snail.hh"

struct Solution {
    enum { kOk, kNotEnough, } status;
    double answer;
};

Solution findClosestDistance(std::vector<Snail> snails);

#endif	// SOLVE_HH
