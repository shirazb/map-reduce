#include <sb-mapreduce/common.h>

#include <string>
#include <fstream>
#include <exception>
#include <iostream>

namespace shiraz::MapReduce::utils {

void log_file(
        const std::string file_path,
        int num_words
) {
    std::ifstream ifs{file_path};
    if (!ifs) {
        throw std::invalid_argument("log_file(): Could not open for reading: "
                + file_path
        );
    }

    for (int i = 0; i < num_words; i++) {
        std::string word;
        ifs >> word;
        std::cout << word << std::endl;
    }
}

}
