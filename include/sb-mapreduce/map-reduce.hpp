// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <list>
#include <unordered_map>
#include <initializer_list>
#include <string>

namespace SBMapReduce {

template<typename K1, typename V1, typename K2, typename V2, typename V3>
class MapReduce {
public:
    void emit_intermediate(K2 key, V2 value);

    void emit(V3 result);

    void execute(std::initializer_list<std::pair<K1, V1>> data);

    using result_stringifier = std::string(*)(V3);
    void dump_results(result_stringifier to_string);

private:
    virtual void map(K1 key, V1 value) = 0;
    virtual void reduce(K2 key, std::list<V2> values) = 0;

    std::unordered_map<K2, std::list<V2>> intermediates;
    std::list<V3> results;
};

}

#include "map-reduce.tpp"
