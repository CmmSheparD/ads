#!/usr/bin/env python3
from re import match
from pathlib import Path

import matplotlib.pyplot as plt


if __name__ == '__main__':
    base = Path('.')
    results = {}
    for case in filter(lambda x: x.is_dir(), base.iterdir()):
        with (case / 'result.txt').open() as f:
            res = {}
            for line in f.readlines():
                matches = match(r'.* (\d+) .* (\d+)ms$', line)
                if matches:
                    res[int(matches[1])] = int(matches[2])
            for n, time in res.items():
                results[n] = (*results[n], time) if n in results else (time,)
    worst = {}
    average = {}
    best = {}
    for n, times in results.items():
        worst[n] = max(times)
        average[n] = sum(times) / len(times)
        best[n] = min(times)
    plt.plot(*zip(*best.items()), 'C3', label='Best')
    plt.plot(*zip(*average.items()), 'C1', label='Average')
    plt.plot(*zip(*worst.items()), 'b', label='Worst')
    plt.legend()
    plt.xlabel('Number of entries')
    plt.ylabel('Calculation time, ms')
    plt.show()
