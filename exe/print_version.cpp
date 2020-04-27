#include <iostream>

#include <sb-mapreduce/version.h>
#include <sb-mapreduce/map-reduce.h>

#include <numeric>

class SomeComp: public SBMapReduce::MapReduce {
    void map(int key, int value) override {
        this->emit_intermediate(key+1, value*2);
    }

    void reduce(int key, std::list<int> values) override {
        const int sum = std::accumulate(values.begin(), values.end(), 0);
        this->emit({key, sum});
    }
};

int main() {
    std::cout << "Hello, world!" << std::endl;
    std::cout << "Version: " << SB_MAPREDUCE_VERSION << std::endl;
    std::cout << std::endl;

    SomeComp mr;
    mr.execute({1, 2, 3, 3, 4, 5, 6});
    mr.dump_results();
}
