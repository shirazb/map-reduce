// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/map-reduce.h>

#include <iostream>

namespace SBMapReduce {

void MapReduce::emit_intermediate(int key, int value) {
    this->intermediates[key].emplace_back(value);
}

void MapReduce::execute(std::initializer_list<int> data) {
    /*
     * Usually, this function will take a single document that will be sent to
     * map. Later, it will be a distributed document, which we will split and
     * send to multiple map functions, which still take the type of a single
     * document, though.
     *
     * Here, for convenience, our "document" is a list of ints, and each map
     * function operates not on a (presumably smaller) list of ints, but
     * on a single int.
     */

    for (auto x: data) {
        this->map(x, x);
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
