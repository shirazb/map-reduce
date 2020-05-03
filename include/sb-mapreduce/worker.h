// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include "master.h"

namespace shiraz::MapReduce {

class Worker {
public:
    explicit Worker(int id) : id{id} {}

    void map_task(UserMapFunc map_f, InputFileIterator input_file_iterators);

    struct Hash {
        std::size_t operator()(const Worker& w) const {
            return std::hash<int>{}(w.id);
        }
    };

    bool operator==(const Worker& w) const {
        return this->id == w.id;
    }

private:
    int id;
};



} // namespace shiraz::MapReduce
