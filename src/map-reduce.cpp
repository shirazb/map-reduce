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
     *
     * Similarly, for reduce, we have simplified. Usually, the function will
     * emit a document, that may be distributed in nature. Instead, we emit a
     * list. We also assume the type of that list - pair(int, int).
     */

    for (auto x: data) {
        this->map(x, x);
    }

    // this->intermediates now populated

    for (const auto& pair: this->intermediates) {
        this->reduce(pair.first, pair.second);
    }
}

void MapReduce::dump_results() {
    for (const auto& pair: this->results) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

void MapReduce::emit(std::pair<int, int> result) {
    this->results.emplace_back(result);
}

}
