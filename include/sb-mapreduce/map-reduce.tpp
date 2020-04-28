// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include "map-reduce.hpp"

namespace SBMapReduce {

template<typename K1, typename V1, typename K2, typename V2, typename V3>
void MapReduce<K1, V1, K2, V2, V3>::emit_intermediate(K2 key, V2 value) {
    this->intermediates[key].emplace_back(value);
}

template<typename K1, typename V1, typename K2, typename V2, typename V3>
void MapReduce<K1, V1, K2, V2, V3>::execute(
        std::initializer_list<std::pair<K1, V1>> data
) {
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

    for (const auto& pair: data) {
        this->map(pair.first, pair.second);
    }

    // this->intermediates now populated

    for (const auto& pair: this->intermediates) {
        this->reduce(pair.first, pair.second);
    }

    // this->results now populated
}

template<typename K1, typename V1, typename K2, typename V2, typename V3>
void MapReduce<K1, V1, K2, V2, V3>::dump_results(result_stringifier to_string) {
    for (const auto& res: this->results) {
        std::cout << to_string(res) << std::endl;
    }
}

template<typename K1, typename V1, typename K2, typename V2, typename V3>
void MapReduce<K1, V1, K2, V2, V3>::emit(V3 result) {
    this->results.emplace_back(result);
}

}
