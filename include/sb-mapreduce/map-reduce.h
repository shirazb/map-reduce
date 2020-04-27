// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <list>
#include <unordered_map>
#include <initializer_list>

namespace SBMapReduce {

class MapReduce {
public:
    void emit_intermediate(int key, int value);

    void emit(std::pair<int, int> result);

    void execute(std::initializer_list<int> data);

    void dump_results();

private:
    virtual void map(int key, int value) = 0;
    virtual void reduce(int key, std::list<int> values) = 0;

    std::unordered_map<int, std::list<int>> intermediates;
    std::list<std::pair<int, int>> results;
};

}
