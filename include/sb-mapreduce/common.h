#pragma once

#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <utility>

namespace shiraz::MapReduce {

class EmitIntermediateStream;
class EmitResultStream;

using InputFileStreams = std::vector<std::ifstream>;
using OutputFileStreams = std::vector<std::ofstream>;

using UserMapFunc = void(*)(std::ifstream&, EmitIntermediateStream&);
using UserReduceFunc = void(*)(std::string, std::list<std::string>, EmitResultStream&);

using IntermediateHashFunc = int(*)(int);


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

class EmitIntermediateStream: public std::ofstream {
public:
    using std::ofstream::ofstream;

    /**
     * For convenience, and to hide type of output iterator created. In
     * particular, hide the delimiter parameter passed to constructor.
     */
    template<typename K_i = std::string, typename V_i = std::string>
    std::ostream_iterator<IntermediateResult<K_i, V_i>>
    begin() {
        return std::ostream_iterator<IntermediateResult<K_i, V_i>>{*this};
    }
};

class EmitResultStream: public std::ofstream {
    using std::ofstream::ofstream;

    template<typename V_r = std::string>
    std::ostream_iterator<V_r>
    begin(const char delim = '\n') {
        return std::ostream_iterator<V_r>{*this, delim};
    }
};

namespace utils {

void
log_file(
        const std::string file_path,
        int num_words = 30,
        char get_line_delim = '\n'
);

} // namespace ::utils
} // namespace shiraz::MapReduce