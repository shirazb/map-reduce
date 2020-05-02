// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once


#include <vector>
#include <string>

namespace shiraz::MapReduce {

using UserMapFunc = void(*)(int, int);
using IntermediateHashFunc = int (*)(int);

class Master {
    Master(
            std::vector<std::string> input_files,
            std::vector<std::string> output_files,
            UserMapFunc map_f,
            int num_workers,
            IntermediateHashFunc intermediate_hash
    );

    void go();

private:
    // file stream iterators
    std::vector<std::string> input_files; // size M
    std::vector<std::string> output_files; // size R

    UserMapFunc map_f{};
    //UserReduceFunc

    int num_workers;

    IntermediateHashFunc intermediate_hash;
};

} // namespace shiraz::MapReduce
