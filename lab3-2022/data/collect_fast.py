#!/usr/bin/env python3
from re import match
from pathlib import Path

import matplotlib.pyplot as plt

def collect_data_on(name: str):
    base = Path('.')
    results = {}
    for case in filter(lambda x: x.is_dir(), base.iterdir()):
        with (case / f'{name}.txt').open() as f:
            res = {}
            for line in f.readlines():
                matches = match(r'^(\d+) .* (\d+)ms$', line)
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
    return (worst, average, best)

if __name__ == '__main__':
    merge = collect_data_on('merge')[1]
    quick = collect_data_on('quick')[1]
    stl = collect_data_on('stl')[1]
    plt.plot(*zip(*merge.items()), label='Merge')
    plt.plot(*zip(*quick.items()), label='Quick')
    plt.plot(*zip(*stl.items()), label='STL')
    plt.legend()
    plt.xlabel('Number of entries')
    plt.ylabel('Calculation time, ms')
    plt.show()
