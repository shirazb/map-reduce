// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/worker.h>

#include <iterator>
#include <fstream>
#include <filesystem>

namespace shiraz::MapReduce {

std::string
Worker::map_task(
        UserMapFunc map_f,
        InputFileIterator input_file_iterator
) {
    (void)map_f;
    (void)input_file_iterator;

    /* Intermediate output file stream iterator */
    
    // NB: temp_directory_path() not thread-safe
    const std::string intermediate_file_dir = std::filesystem::temp_directory_path();
    const std::string intermediate_file_path = intermediate_file_dir + 
            "sb-mapreduce-intermediate-output--worker-" + 
            std::to_string(this->id);

    // TODO: Handle opening error.
    std::ofstream ofs{intermediate_file_path};
    OutputFileIterator ofs_it{ofs};

    /* Do stuff, populating ofs_it */

    return intermediate_file_path;
}

} // namespace shiraz::MapReduce
