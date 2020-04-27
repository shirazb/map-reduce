// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <list>
#include <unordered_map>
#include <initializer_list>

namespace SBMapReduce {

class MapReduce {
public:
    void emit_intermediate(int key, int value);

    void execute(std::initializer_list<int> data);

    void dump_intermediates_cout();

private:
    virtual void map(int key, int value) = 0;

    std::unordered_map<int, std::list<int>> intermediates;
};

}
