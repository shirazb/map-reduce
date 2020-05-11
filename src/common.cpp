#include <sb-mapreduce/common.h>

#include <stdexcept>
#include <string>
#include <iostream>

namespace shiraz::MapReduce::utils {

void
log_file(
        const std::string& file_path,
        const int n,
        const char delim,
        std::ostream& os
) {
    std::ifstream ifs{file_path};
    if (!ifs) {
        throw std::invalid_argument("log_file(): Could not open for reading: "
                + file_path
        );
    }

    std::cout << "log_file(): " << file_path << std::endl;

    for (int i = 0; i < n; i++) {
        std::string word;
        std::getline(ifs, word, delim);
        os << word << std::endl;
    }
}

}
