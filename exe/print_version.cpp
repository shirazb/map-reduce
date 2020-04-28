#include <iostream>
#include <sstream>

#include <sb-mapreduce/version.h>
#include <sb-mapreduce/map-reduce.hpp>

#include <numeric>

using SomeMapReduceType = SBMapReduce::MapReduce<int, int, int, int, std::pair<int, int>>;

class SomeComp: public SomeMapReduceType {
    void map(int key, int value) override {
        this->emit_intermediate(value+1, key*2);
    }

    void reduce(int key, std::list<int> values) override {
        const int sum = std::accumulate(values.begin(), values.end(), 0);
        this->emit(std::pair<int, int>{key, sum});
    }

public:
    static std::string result_to_string(std::pair<int, int> res) {
        std::ostringstream res_str;
        res_str << res.first << ": " << res.second;

        return res_str.str();
    }
};

int main() {
    std::cout << "Hello, world!" << std::endl;
    std::cout << "Version: " << SB_MAPREDUCE_VERSION << std::endl;
    std::cout << std::endl;

    SomeComp mr;
    mr.execute({
           {1, 2},
           {2, 4},
           {3, 6},
           {1, 4},
           {7, 8}
    });

    mr.dump_results(SomeComp::result_to_string);
}
