// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/worker.h>
#include <sb-mapreduce/common.h>
#include <sb-mapreduce/emit-streams.h>

#include <iterator>
#include <fstream>
#include <filesystem>
#include <string>
#include <list>
#include <unordered_map>
#include <sstream>

namespace {

void
parse_intermediate_entry(
        const std::string& s,
        std::string& ikey,
        std::string& ivalue
);

struct inter_file;
struct user_file;
}

namespace shiraz::MapReduce {

std::vector<std::string>
Worker::map_task(
        const int map_task_no,
        UserMapFunc map_f,
        const std::string& input_fp,
        IntermediateHashFunc hash_inter
) {
    /* Input input file stream */
    auto input_ifs = this->try_open_file_or_throw<std::ifstream, user_file>(input_fp);

    /* Create emitter from R intermediate output file streams */

    const int R = this->num_reduce_tasks;
    const int m = map_task_no;

    std::vector<std::ofstream> inter_ofs;
    std::vector<std::string> inter_fps;

    for (int r = 0; r < R; r++) {
        const auto fp = this->get_intermediate_fp(m ,r);
        inter_ofs.emplace_back(
                this->try_open_file_or_throw<std::ofstream, inter_file>(fp)
        );
        inter_fps.push_back(fp);
    }

    EmitIntermediateStream emit_s{hash_inter, std::move(inter_ofs)};

    /* Do stuff, writing to ofs. */

    map_f(input_ifs, emit_s);

    return inter_fps;
}

void
Worker::reduce_task(
        UserReduceFunc reduce_f,
        const std::string& intermediate_fp,
        const std::string& output_fp
) {
    /* Build intermediates map. */

    std::unordered_map<std::string, std::list<std::string>> intermediates;

    // Construct iterator of intermediate file.
    auto intermediate_ifs = try_open_file_or_throw<std::ifstream, inter_file>(
            intermediate_fp
    );
    std::istream_iterator<std::string> intermediate_ifs_it{intermediate_ifs};
    std::istream_iterator<std::string> eos_it{};

    std::for_each(intermediate_ifs_it, eos_it,
            [&intermediates](const auto& line){
                std::string ikey, ivalue;
                parse_intermediate_entry(line, ikey, ivalue);

                intermediates[ikey].emplace_back(std::move(ivalue));
        }
    );

    /* Run user reduce func on each key */

    // TODO: Inconsistent level of abstraction that we specify open mode here
    // but not for EmitIntermediateStream.
    auto emit_s = try_open_file_or_throw<EmitResultStream, user_file>(
            output_fp,
            std::ofstream::out | std::ofstream::trunc
    );

    const auto dispatch_reduce_f = [&](auto const& pair) { 
            reduce_f(pair.first, pair.second, emit_s);
    };

    std::for_each(intermediates.begin(), intermediates.end(), dispatch_reduce_f);
}

/**
 * S models a derived type of `open_file_type`.
 * T_fstream models the type of std::fstream to open, e.g. std::ifstream.
 * ...Param_ifs are types of parameters to std::ifstream after first (string 
 * filepath) parameter.
 * 
 * Tries to open a std::ifstream on a file using the provided arguments, which
 * are passed directly to the std::ifstream constructor.
 * 
 * `fp`: Filepath to open.
 * `args...` remaining arguments to std::ifstream constructor (optional).
 * 
 * Depending on `S`, on failure will throw the appropriate exception. That is,
 * depending on whether or not we are opening a user-provided file path or an
 * internally generated intermediate file path.
 */
template<typename T_fstream, typename S, typename ...Params_ifs>
T_fstream
Worker::try_open_file_or_throw(const std::string& fp, Params_ifs... args, ...) {
    T_fstream fs(fp, args...);
    if (!fs) {
        // See `struct open_file_type` for `exception_type` description.
        throw typename S::exception_type{
            std::to_string(this->id), fp
        };
    }

    return fs;
}

std::string
Worker::get_intermediate_fp(const int m, const int r) {
    // NB: temp_directory_path() not thread-safe
    static const std::string intermediate_file_dir =
            std::filesystem::temp_directory_path();

    return intermediate_file_dir + "/sb-mapreduce-intermediate-output-worker-"
            + std::to_string(this->id) + "--bucket-"
            + std::to_string(m) + "-" + std::to_string(r);
}

Worker::FailedToOpenUserFileException::FailedToOpenUserFileException(
        const std::string& who,
        const std::string& fp
) : 
        std::invalid_argument(build_error_str(who, fp)) {}

std::string
Worker::FailedToOpenUserFileException::build_error_str(
        const std::string& who,
        const std::string& fp
) {
    std::ostringstream ss;
    ss << "shiraz::MapReduce: Worker `" << who << "` failed to read user-" << 
            "provided file `" << fp << "`.";
    return ss.str();
}

Worker::FailedToOpenIntermediateFileException::FailedToOpenIntermediateFileException(
        const std::string& who,
        const std::string& fp
) :
        std::logic_error(build_error_str(who, fp)) {}

std::string
Worker::FailedToOpenIntermediateFileException::build_error_str(
        const std::string& who,
        const std::string& fp
) {
    std::ostringstream ss;
    ss << "shiraz::MapReduce INTERNAL ERROR (please report): " <<
            "Worker `" << who << "` failed to read intermediate file `" <<
            fp << "`.";
    return ss.str();
}

} // namespace shiraz::MapReduce


using namespace shiraz::MapReduce;

namespace {

/**
 * Precond: `s` is of form "k,v\n". That is, with NO spacing around the comma
 * and a newline character at the end.
 */
void
parse_intermediate_entry(
        const std::string& s,
        std::string& ikey,
        std::string& ivalue
) {
    // FIXME: User cannot put comma in key.
    std::istringstream ss(s);
    std::getline(ss, ikey, ',');
    std::getline(ss, ivalue);
}


/**
 * For use by try_open_file_or_throw
 * 
 * Must declare an `exception_type` type that is an exception. The constructor
 * of this exception will have the following signature:
 * 
 *     exception_type(const std::string& who, const std::string& fp)
 * 
 * where `who` describes the worker who has failed to open a file stream; and
 * `fp` is the file path that it failed to open.
 */
struct inter_file {
    using exception_type = Worker::FailedToOpenIntermediateFileException;
};
struct user_file {
    using exception_type = Worker::FailedToOpenUserFileException;
};

}
