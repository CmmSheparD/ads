#include <fstream>
#include <random>
#include <vector>

using namespace std;

const size_t kN = 5000000;

int main()
{
    vector<int> data;
    default_random_engine engine((random_device())());
    uniform_int_distribution<int> dis;
    for (size_t i = 0; i < kN; ++i) {
        data.push_back(dis(engine));
    }
    ofstream out("data.txt");
    out << kN << '\n';
    for (size_t i = 0; i < kN; ++i) {
        out << data[i];
        if (i != kN) out << ' ';
    }
    out << '\n';
    out.close();
    return 0;
}
