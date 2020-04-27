#include <iostream>

#include <sb-mapreduce/version.h>
#include <sb-mapreduce/map-reduce.h>

class SomeMap: public SBMapReduce::MapReduce {
    void map_func(int key, int value) override {
        this->emit_intermediate(key+1, value*2);
    }
};

int main() {
    std::cout << "Hello, world!" << std::endl;
    std::cout << "Version: " << SB_MAPREDUCE_VERSION << std::endl;

    SomeMap map;
    map.map({1, 2, 3, 3, 4, 5, 6});
    map.dump_intermediates_cout();
}
