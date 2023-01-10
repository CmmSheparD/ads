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
    insertion = collect_data_on('insertion')[1]
    selection = collect_data_on('selection')[1]
    bubble = collect_data_on('bubble')[1]
    shell = collect_data_on('shell')[1]
    plt.plot(*zip(*insertion.items()), label='Insertion')
    plt.plot(*zip(*selection.items()), label='Selection')
    plt.plot(*zip(*bubble.items()), label='Bubble')
    plt.plot(*zip(*shell.items()), label='Shell')
    plt.legend()
    plt.xlabel('Number of entries')
    plt.ylabel('Calculation time, ms')
    plt.show()
