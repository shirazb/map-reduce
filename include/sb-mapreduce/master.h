// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <iterator>
#include <exception>
#include <vector>
#include <string>
#include <fstream>

namespace shiraz::MapReduce {

struct IntermediateEmitter;

using InputFileIterator = std::istream_iterator<std::string>;
using OutputFileIterator = std::ostream_iterator<std::string>;

using UserMapFunc = void(*)(std::string, std::string, IntermediateEmitter);

using IntermediateHashFunc = int(*)(int);

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

struct IntermediateEmitter {
public:
    IntermediateEmitter(std::ofstream intermediate_ofs):
            intermediate_ofs{std::move(intermediate_ofs)} {}

    void operator()(const std::string ikey, const std::string ivalue) {
        intermediate_ofs << ikey << "," << ivalue << std::endl;
    }

private:
    std::ofstream intermediate_ofs;
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
