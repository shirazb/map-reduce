// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <iterator>
#include <exception>
#include <vector>
#include <string>
#include <sstream>

namespace shiraz::MapReduce {

using InputFileIterator = std::istream_iterator<std::string>;
using OutputFileIterator = std::ostream_iterator<std::string>;

using UserMapFunc = void(*)(int, int);

using IntermediateHashFunc = int (*)(int);

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

private:
    // file stream iterators
    std::vector<InputFileIterator> input_file_iterators; // size M
    std::vector<OutputFileIterator> output_file_iterators; // size R

    UserMapFunc map_f{};
    //UserReduceFunc

    int num_workers;

    IntermediateHashFunc intermediate_hash;
};

struct MasterNotEnoughWorkersException: std::invalid_argument {

    MasterNotEnoughWorkersException(
            std::size_t num_ifstreams,
            std::size_t num_ofstreams,
            int num_workers
    ):
            invalid_argument(build_error_str(
                    num_ifstreams, num_ofstreams, num_workers
            ))
    {}

private:
    static std::string build_error_str(
            std::size_t num_ifstreams,
            std::size_t num_ofstreams,
            int num_workers
    ) {
        std::ostringstream msg;
        msg << "Number workers must be at least the number of ifstreams and " <<
            "number of ofstreams. " <<
            "Number workers = " << num_workers <<
            ". Number ifstreams = " << num_ifstreams <<
            ". Number ofstreams = " << num_ofstreams;

        return msg.str();
    }
};

} // namespace shiraz::MapReduce
