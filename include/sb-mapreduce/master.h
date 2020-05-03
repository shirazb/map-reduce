// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <sb-mapreduce/common.h>

#include <iterator>
#include <exception>
#include <vector>
#include <string>
#include <fstream>

namespace shiraz::MapReduce {

class Master {
public:
    Master(
            std::vector<InputFileIterator> input_file_iterators,
            std::vector<OutputFileIterator> output_file_iterators,
            UserMapFunc map_f,
            int num_workers,
            IntermediateHashFunc intermediate_hash
    );

    Master(const Master& m) =delete;
    Master& operator=(const Master& m) =delete;

    Master(Master&& m) =default;
    Master& operator=(Master&& m) =default;

    void go();

    struct NotEnoughWorkersException;

private:
    // file stream iterators
    std::vector<InputFileIterator> input_file_iterators; // size M
    std::vector<OutputFileIterator> output_file_iterators; // size R

    UserMapFunc map_f;
    //UserReduceFunc

    int num_workers;

    IntermediateHashFunc intermediate_hash;
};

struct Master::NotEnoughWorkersException: std::invalid_argument {
    NotEnoughWorkersException(
            std::size_t num_ifstreams,
            std::size_t num_ofstreams,
            int num_workers
    );

private:
    static std::string build_error_str(
            std::size_t num_ifstreams,
            std::size_t num_ofstreams,
            int num_workers
    );
};

} // namespace shiraz::MapReduce
