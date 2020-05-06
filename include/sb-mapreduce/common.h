#pragma once

#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <utility>

namespace shiraz::MapReduce {

class EmitIntermediateStream;
using EmitResultStream = std::ofstream;

using InputFilePaths = std::vector<std::string>;
using OutputFilePaths = std::vector<std::string>;

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

class EmitIntermediateStream {
public:
    EmitIntermediateStream(const std::string& fp) :
            ofs{std::ofstream{fp,
                std::ofstream::out | std::ofstream::trunc
            }}
    {}

    /**
     * For convenience, and to hide type of output iterator created. In
     * particular, hide the delimiter parameter passed to constructor.
     */
    template<typename K_i = std::string, typename V_i = std::string>
    std::ostream_iterator<IntermediateResult<K_i, V_i>>
    begin() {
        return std::ostream_iterator<IntermediateResult<K_i, V_i>>{this->ofs};
    }

    template<typename K_i, typename V_i>
    EmitIntermediateStream&
    operator<<(IntermediateResult<K_i, V_i> ir) {
        this->ofs << ir.first << "," << ir.second << std::endl;
        return *this;
    }

    template<typename K_i, typename V_i>
    void
    to_stream(IntermediateResult<K_i, V_i> ir) {
        this << ir;
    }

    bool
    operator!() const {
        return !this->ofs;
    }

    explicit
    operator bool() const {
        return (bool) this->ofs;
    }

    // Is movable, not copyable. Copy already deleted in superclass.
    EmitIntermediateStream(EmitIntermediateStream&&) =default;
    EmitIntermediateStream& operator=(EmitIntermediateStream&&) =default;

private:
    std::ofstream ofs;
};

namespace utils {

void
log_file(
        const std::string& file_path,
        int num_words = 30,
        char get_line_delim = '\n'
);

} // namespace ::utils
} // namespace shiraz::MapReduce