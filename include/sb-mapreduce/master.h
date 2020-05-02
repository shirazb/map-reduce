// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once


#include <vector>
#include <string>

namespace shiraz::MapReduce {

using UserMapFunc = void(*)(int, int);
using IntermediateHashFunc = int (*)(int);

class Master {
public:
    Master(
            std::vector<std::string> input_files,
            std::vector<std::string> output_files,
            UserMapFunc map_f,
            int num_workers,
            IntermediateHashFunc intermediate_hash
    );

    Master(const Master& m) =delete;
    Master& operator=(const Master& m) =delete;

    Master(Master&& m) =default;
    Master& operator=(Master&& m) =default;

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
