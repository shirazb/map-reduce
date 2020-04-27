#include <iostream>

#include <sb-mapreduce/version.h>
#include <sb-mapreduce/map-reduce.h>

class SomeComp: public SBMapReduce::MapReduce {
    void map(int key, int value) override {
        this->emit_intermediate(key+1, value*2);
    }
};

int main() {
    std::cout << "Hello, world!" << std::endl;
    std::cout << "Version: " << SB_MAPREDUCE_VERSION << std::endl;

    SomeComp map;
    map.execute({1, 2, 3, 3, 4, 5, 6});
    map.dump_intermediates_cout();
}
