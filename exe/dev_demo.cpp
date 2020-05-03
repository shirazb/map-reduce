// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/version.h>
#include <sb-mapreduce/master.h>

#include <string>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>

/*********************** prototypes *******************************************/

namespace {

// CWD must be map-reduce root in build dir.

const std::string data_root_path = "data/";
const std::string input_file_name = "pg-being_ernest.txt";
const std::string input_file_path = data_root_path + input_file_name;

const std::string temp_data_dir_path = data_root_path + "tmp/";
const std::string preproc_files_prefix = "preprocessed_words_only_";

const std::string preproc_file_path = temp_data_dir_path + preproc_files_prefix
        + input_file_name;

void preprocess_input_file(
        const std::string input_file_path,
        const std::string preproc_file_path
);

std::string remove_punctuation(std::string s);
}

/*********************** main() ***********************************************/

int main() {
    std::cout << "sb-mapreduce" << std::endl;
    std::cout << "Version: " << SB_MAPREDUCE_VERSION << std::endl;

    std::cout << std::endl << std::endl;

    preprocess_input_file(input_file_path, preproc_file_path);

    // Done: log preprocessed file.

    std::ifstream preproc_ifs{preproc_file_path};
    if (!preproc_ifs) {
        throw std::invalid_argument("Could not open preproc file for reading: "
                + preproc_file_path
        );
    }

    for (int i = 0; i < 40; i++) {
        std::string word;
        preproc_ifs >> word;
        std::cout << word << std::endl;
    }
}

/*********************** helpers **********************************************/

namespace {

void preprocess_input_file(
        const std::string input_file_path,
        const std::string preproc_file_path
) {
    std::ifstream ifs{input_file_path};
    if (!ifs) {
        throw std::invalid_argument(
                "Could not open input file for reading: " + input_file_path
        );
    }

    std::ofstream ofs{preproc_file_path};
    if (!ofs) {
        throw std::invalid_argument(
                "Could not open preprocessed input file for writing: " + 
                    preproc_file_path
        );
    }

    std::istream_iterator<std::string> ifs_it{ifs};
    std::istream_iterator<std::string> end_ifs_it{}; // EOS sentinel
    std::ostream_iterator<std::string> ofs_it{ofs};

    std::transform(ifs_it, end_ifs_it, ofs_it, remove_punctuation);
}

std::string remove_punctuation(std::string s) {
    s.erase(
        std::remove_if(s.begin(), s.end(), ::ispunct),
        s.end()
    );

    return s + ' ';
}

}