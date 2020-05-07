// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <sb-mapreduce/common.h>

#include <fstream>

namespace shiraz::MapReduce {

class Worker {
public:
    explicit Worker(int id) : id{id} {}

    // Not copyable.
    Worker(const Worker& w) =delete;
    Worker& operator=(const Worker& w) =delete;

    // Is movable.
    Worker(Worker&& w) =default;
    Worker& operator=(Worker&& w) =default;

    std::string
    map_task(
            UserMapFunc map_f,
            const std::string& input_fp
    );

    void
    reduce_task(
            UserReduceFunc reduce_f,
            const std::string& intermediate_fp,
            const std::string& output_fp
    );

    struct Hash {
        std::size_t operator()(const Worker& w) const {
            return std::hash<int>{}(w.id);
        }
    };

    bool operator==(const Worker& w) const {
        return this->id == w.id;
    }

    struct FailedToOpenUserFileException;
    struct FailedToOpenIntermediateFileException;

private:
    int id;

    template<typename T_fstream, typename S, typename ...Params_ifs>
    T_fstream
    try_open_file_or_throw(
            const std::string& fp,
            Params_ifs... args, ...
    );
};

struct Worker::FailedToOpenUserFileException: public std::invalid_argument {
    FailedToOpenUserFileException(const std::string& who, const std::string& fp);

private:
    static
    std::string
    build_error_str(const std::string& who, const std::string& fp);
};

struct Worker::FailedToOpenIntermediateFileException: public std::logic_error {
    FailedToOpenIntermediateFileException(
            const std::string& who,
            const std::string& fp
    );

private:
    static
    std::string
    build_error_str(const std::string& who, const std::string& fp);
};


} // namespace shiraz::MapReduce
