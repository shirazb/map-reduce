// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/map-reduce.h>

#include <iostream>

namespace SBMapReduce {

void MapReduce::emit_intermediate(int key, int value) {
    this->intermediates[key].emplace_back(value);
}

void MapReduce::map(std::initializer_list<int> data) {
    for (auto x: data) {
        this->map_func(x, x);
    }
}

void MapReduce::dump_intermediates_cout() {
    for (const auto& pair: this->intermediates) {
        std::cout << pair.first << ": ";
        for (auto val: pair.second) {
            std::cout << val << ", ";
        }
        std::cout << std::endl;
    }
}

}
