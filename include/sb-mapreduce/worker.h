// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <sb-mapreduce/common.h>

namespace shiraz::MapReduce {

class Worker {
public:
    explicit Worker(int id) : id{id} {}

    std::string
    map_task(UserMapFunc map_f, InputFileIterator input_file_iterator);

    void
    reduce_task(UserReduceFunc reduce_f, InputFileIterator intermediate_file_it, 
            OutputFileIterator output_file_it);

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
