#pragma once

#include <any>
#include <cstddef>
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <functional>

namespace shiraz::MapReduce {

/* Used internally to represent 1D slice (view) of 2D vector of strings. */
using _vec_of_const_str_ref =
        std::vector<std::reference_wrapper<const std::string>>;

class EmitIntermediateStream;
using EmitResultStream = std::ofstream;

using InputFilePaths = std::vector<std::string>;
using OutputFilePaths = std::vector<std::string>;

using UserMapFunc = void(*)(std::ifstream&, EmitIntermediateStream&);
using UserReduceFunc = void(*)(std::string, std::list<std::string>, EmitResultStream&);

using IntermediateHashFunc = std::size_t(*)(std::any);

template<typename K_i = std::string, typename V_i = std::string>
class IntermediateResult: public std::pair<K_i, V_i> {
public:
    using std::pair<K_i, V_i>::pair;
};

template<typename K_i, typename V_i>
std::ostream&
operator<<(std::ostream& os, const IntermediateResult<K_i, V_i>& itr) {
    os << itr.first << "," << itr.second << std::endl;
    return os;
}

namespace utils {

void
log_file(
        const std::string& file_path,
        int num_words = 30,
        char get_line_delim = '\n'
);

} // namespace ::utils
} // namespace shiraz::MapReduce
