// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/worker.h>
#include <sb-mapreduce/common.h>

#include <iterator>
#include <fstream>
#include <filesystem>
#include <string>
#include <list>
#include <unordered_map>
#include <sstream>

// Only indent 4 beacuse of stupid editors like VS Code
namespace {

void
parse_intermediate_entry(
    const std::string s,
    std::string& ikey,
    std::string& ivalue
);
}

namespace shiraz::MapReduce {

std::string
Worker::map_task(
        UserMapFunc map_f,
        std::ifstream& input_ifs
) {
    /* Intermediate output file stream iterator */
    
    // NB: temp_directory_path() not thread-safe
    const std::string intermediate_file_dir = std::filesystem::temp_directory_path();
    const std::string intermediate_file_path = intermediate_file_dir + 
            "/sb-mapreduce-intermediate-output--worker-" + 
            std::to_string(this->id);

    // TODO: Handle opening error.
    // Create IntermediateEmitter function passed to user map_f that will
    // stream output to file.
    std::ofstream intermediate_ofs{intermediate_file_path,
            std::ofstream::out | std::ofstream::trunc
    };
    IntermediateEmitter emit_intermediate{intermediate_ofs};

    /* Do stuff, populating ofs_it */

    std::istream_iterator<std::string> input_ifs_it{input_ifs};
    std::istream_iterator<std::string> eos_it{};

    std::for_each(input_ifs_it, eos_it, 
            [&](auto&& s) { map_f(s, emit_intermediate); }
    );

    return intermediate_file_path;
}

void
Worker::reduce_task(
        UserReduceFunc reduce_f,
        std::ifstream& intermediate_ifs,
        std::ofstream& output_ofs
) {
    /* Build intermediates map. */

    std::unordered_map<std::string, std::list<std::string>> intermediates;

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
    ResultEmitter emit{output_ofs};

    const auto dispatch_reduce_f = [&](auto const& pair) { 
            reduce_f(pair.first, pair.second, emit);
    };

    std::for_each(intermediates.begin(), intermediates.end(), dispatch_reduce_f);
}

} // namespace shiraz::MapReduce

namespace {

/**
 * Precond: `s` is of form "k,v\n". That is, with NO spacing around the comma
 * and a newline character at the end.
 */
void
parse_intermediate_entry(
        const std::string s,
        std::string& ikey,
        std::string& ivalue
) {
    // FIXME: User cannot put comma in key.
    std::istringstream ss(s);
    std::getline(ss, ikey, ',');
    std::getline(ss, ivalue);
}

}