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
    results = (
        results.keys(),
        [sum(times) / len(times) for times in results.values()]
    )
    plt.plot(*results)
    plt.xlabel('Number of entries')
    plt.ylabel('Average calculation time, ms')
    plt.show()
