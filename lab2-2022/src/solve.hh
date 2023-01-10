#pragma once
#ifndef SOLVE_HH
#define SOLVE_HH

#include <vector>

#include "snail.hh"

enum SolutionStatus {
    kOk,
    kNotEnough, 
};

struct Solution {
    Solution() : answer(-1), status(kNotEnough) {}
    Solution(SolutionStatus s, double ans)
        : answer(ans), status(s)
    {}
    double answer;
    SolutionStatus status;
};

Solution findClosestDistance(std::vector<Snail> snails);

#endif	// SOLVE_HH
