#pragma once

#include <string>
#include <fstream>
#include <list>
#include <vector>

namespace shiraz::MapReduce {

struct IntermediateEmitter;
struct ResultEmitter;


using InputFileStreams = std::vector<std::ifstream>;
using OutputFileStreams = std::vector<std::ofstream>;

using UserMapFunc = void(*)(std::ifstream&, IntermediateEmitter&);
using UserReduceFunc = void(*)(std::string, std::list<std::string>, ResultEmitter&);

using IntermediateHashFunc = int(*)(int);


struct IntermediateEmitter {
public:
    IntermediateEmitter(
            std::ofstream& intermediate_ofs
    ):
            intermediate_ofs{intermediate_ofs} {}

    void operator()(const std::string ikey, const std::string ivalue);

private:
    std::ofstream& intermediate_ofs;
};

struct ResultEmitter {
public:
    ResultEmitter(
            std::ofstream& output_ofs
    ):
            output_ofs{output_ofs} {}

    void operator()(const std::string resvalue);

private:
    std::ofstream& output_ofs;
};

}

namespace shiraz::MapReduce::utils {

void
log_file(
        const std::string file_path,
        int num_words = 30,
        char get_line_delim = '\n'
);


}
