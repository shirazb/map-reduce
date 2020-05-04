#pragma once

#include <string>
#include <fstream>
#include <list>

namespace shiraz::MapReduce {

struct IntermediateEmitter;
struct ResultEmitter;

using InputFileIterator = std::istream_iterator<std::string>;
using OutputFileIterator = std::ostream_iterator<std::string>;

using UserMapFunc = void(*)(std::string, IntermediateEmitter&);
using UserReduceFunc = void(*)(std::string, std::list<std::string>, ResultEmitter&);

using IntermediateHashFunc = int(*)(int);


struct IntermediateEmitter {
public:
    IntermediateEmitter(
            std::ofstream intermediate_ofs
    ):
            intermediate_ofs{std::move(intermediate_ofs)} {}

    void operator()(const std::string ikey, const std::string ivalue);

private:
    std::ofstream intermediate_ofs;
};

struct ResultEmitter {
public:
    ResultEmitter(
            OutputFileIterator output_it
    ):
            output_it{std::move(output_it)} {}

    void operator()(const std::string resvalue);

private:
    OutputFileIterator output_it;
};

}

namespace shiraz::MapReduce::utils {

void log_file(
        const std::string file_path,
        int num_words = 30,
        char get_line_delim = '\n'
);


}
