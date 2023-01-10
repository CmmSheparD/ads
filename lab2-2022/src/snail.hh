#pragma once
#ifndef SNAIL_HH
#define SNAIL_HH

struct Snail {
	Snail(double x, double y) : x(x), y(y) {}
    Snail &operator=(const Snail &other) = default;

    double distanceTo(const Snail &other) const;

    bool operator==(const Snail &other) const;

	double x;
	double y;
};

#endif	// SNAIL_HH
