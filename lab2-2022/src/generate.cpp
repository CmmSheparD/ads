#include "generate.hh"

#include <random>

using namespace std;

vector<Snail> generate(size_t n)
{
    vector<Snail> snails(n, {0, 0});
    default_random_engine engine((random_device())());
    uniform_real_distribution<double> r(0, 1000);
    bool equal;
    for (size_t i = 0; i < n; ++i) {
        do {
            equal = false;
            Snail snail(r(engine), r(engine));
            for (size_t j = 0; !equal && j < i; ++j) {
                if (snail == snails[j])
                    equal = true;
            }
            snails[i] = snail;
        } while (equal);
    }
    return snails;
}
