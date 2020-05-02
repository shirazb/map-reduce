// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <iterator>
#include <vector>
#include <string>

namespace shiraz::MapReduce {

using InputFileIterator = std::vector<std::istream_iterator<std::string>>;
using OutputFileIterator = std::vector<std::ostream_iterator<std::string>>;

using UserMapFunc = void(*)(int, int);

using IntermediateHashFunc = int (*)(int);

class Master {
public:
    Master(
            InputFileIterator input_file_iterators,
            OutputFileIterator output_file_iterators,
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
    InputFileIterator input_file_iterators; // size M
    OutputFileIterator output_file_iterators; // size R

    UserMapFunc map_f{};
    //UserReduceFunc

    int num_workers;

    IntermediateHashFunc intermediate_hash;
};

} // namespace shiraz::MapReduce
