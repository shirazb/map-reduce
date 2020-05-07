#pragma once

#include <string>
#include <fstream>
#include <list>
#include <vector>

namespace shiraz::MapReduce {

class EmitIntermediateStream;
using EmitResultStream = std::ofstream;

using InputFilePaths = std::vector<std::string>;
using OutputFilePaths = std::vector<std::string>;

using UserMapFunc = void(*)(std::ifstream&, EmitIntermediateStream&);
using UserReduceFunc = void(*)(std::string, std::list<std::string>, EmitResultStream&);

using IntermediateHashFunc = int(*)(int);

namespace utils {

void
log_file(
        const std::string& file_path,
        int num_words = 30,
        char get_line_delim = '\n'
);

} // namespace ::utils
} // namespace shiraz::MapReduce
