// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/worker.h>
#include <sb-mapreduce/master.h>

#include <iterator>
#include <fstream>
#include <filesystem>

namespace shiraz::MapReduce {

std::string
Worker::map_task(
        UserMapFunc map_f,
        InputFileIterator input_file_iterator
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
    IntermediateEmitter emit_intermediate{std::ofstream{
            intermediate_file_path,
            std::ofstream::out | std::ofstream::trunc
    }};

    /* Do stuff, populating ofs_it */

    InputFileIterator input_file_end{};

    std::for_each(input_file_iterator, input_file_end, 
            [&](auto&& s) { map_f(s, emit_intermediate); }
    );

    return intermediate_file_path;
}

} // namespace shiraz::MapReduce
