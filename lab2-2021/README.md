# Labwork #2

## Building

Build just like any other `cmake` project.

## Targets

### Tests

**{path-to-build}/test** contains test executables: `ads-lab2-searches-test` and `ads-lab2-sorts-test`, which test every algorithm implemented in this labwork.

### Time consumption measurement

**{path-to-build}/test** contains the `ads-lab2-measure` executable. This one generates random vectors with sizes from 10 to 100,000 with step of 10 elements for quick sort and bubble sort and measures sort time 10 times for each size and calculates average.