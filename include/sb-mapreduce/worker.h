// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <sb-mapreduce/common.h>

#include <fstream>

namespace shiraz::MapReduce {

class Worker {
public:
    explicit Worker(int id) : id{id} {}

    // Not copyable.
    Worker(const Worker& w) =delete;
    Worker& operator=(const Worker& w) =delete;

    // Is movable.
    Worker(Worker&& w) =default;
    Worker& operator=(Worker&& w) =default;

    std::string
    map_task(
            UserMapFunc map_f,
            std::ifstream& input_ifs
    );

    void
    reduce_task(
            UserReduceFunc reduce_f,
            std::ifstream& intermediate_ifs, 
            std::ofstream& output_ofs
    );

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
