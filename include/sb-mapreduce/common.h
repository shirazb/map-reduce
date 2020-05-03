#pragma once

#include <string>
#include <fstream>

namespace shiraz::MapReduce::utils {

void log_file(
        const std::string file_path,
        int num_words = 30
);


}

namespace shiraz::MapReduce {

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

using InputFileIterator = std::istream_iterator<std::string>;
using OutputFileIterator = std::ostream_iterator<std::string>;

using UserMapFunc = void(*)(std::string, IntermediateEmitter&);

using IntermediateHashFunc = int(*)(int);

}
