// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/version.h>
#include <sb-mapreduce/master.h>
#include <sb-mapreduce/common.h>

#include <string>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <sstream>

using namespace shiraz;

#define NUM_WORKERS 10

/*********************** prototypes *******************************************/

namespace {

// CWD must be map-reduce root in build dir.

const std::string data_root_path = "data/";

const std::string input_data_dir_path = data_root_path + "inputs/";
const std::string input_file_name = "pg-being_ernest.txt";
const std::string input_file_path = input_data_dir_path + input_file_name;

const std::string temp_data_dir_path = data_root_path + "tmp/";
const std::string preproc_files_prefix = "preprocessed_words_only_";

const std::string preproc_file_path = temp_data_dir_path + preproc_files_prefix
        + input_file_name;

const std::string output_data_dir_path = data_root_path + "outputs/";
const std::string output_file_name = "word_count.txt";
const std::string output_file_path = output_data_dir_path + output_file_name;

void preprocess_input_file(
        const std::string input_file_path,
        const std::string preproc_file_path
);

std::string remove_punctuation(std::string s);


const auto map_f = [](std::string k, MapReduce::IntermediateEmitter& emit) {
    emit(k, std::to_string(1));
};

const auto intermediate_hash = [](int k){ return k % NUM_WORKERS; };

void reduce_f(
        std::string ikey,
        std::list<std::string> ivalues,
        MapReduce::ResultEmitter& emit
) {
    int acc = 0;
    for (auto &iv: ivalues) {
        try {

            acc += std::stoi(iv);

        } catch (std::invalid_argument& ex) {
            std::cout << "reduce_f(): For key '" << ikey << "' could not parse "
                    << "value: " << iv << std::endl
                    << "    Threw std::invalid_argument: " << std::endl
                    << "    " << ex.what();
            continue;
        } catch (std::out_of_range& ex) {
            std::cout << "reduce_f(): For key '" << ikey << "' could not parse "
                    << "value: " << iv << std::endl
                    << "    Threw std::out_of_range: " << std::endl
                    << "    " << ex.what();
            continue;
        }
    }

    std::ostringstream res;
    res << "(" << ikey << ", " << acc << ")";    

    emit(res.str());
};

}

/*********************** main() ***********************************************/

int main() {
    std::cout << "sb-mapreduce" << std::endl;
    std::cout << "Version: " << SB_MAPREDUCE_VERSION << std::endl;
    std::cout << std::endl;

    preprocess_input_file(input_file_path, preproc_file_path);
    std::cout << std::endl;

    // Create and run MapReduce::Master
    {
        std::vector<MapReduce::InputFileIterator> inputs;
        std::ifstream preproc_ifs{preproc_file_path};
        inputs.emplace_back(MapReduce::InputFileIterator{preproc_ifs});

        std::vector<MapReduce::OutputFileIterator> outputs;
        std::ofstream output_ofs{output_file_path, 
                std::ofstream::out | std::ofstream::trunc
        };
        outputs.emplace_back(MapReduce::OutputFileIterator{output_ofs, "\n"});
        
        MapReduce::Master master{
                inputs, outputs,
                map_f, reduce_f,
                NUM_WORKERS,
                intermediate_hash
        };

        master.go();

        std::cout << std::endl << std::endl << "Done master.go()!" << std::endl;
    }

    MapReduce::utils::log_file(output_file_path);
}

/*********************** helpers **********************************************/

namespace {

void preprocess_input_file(
        const std::string input_file_path,
        const std::string preproc_file_path
) {
    // Already done in previous invocation
    if (std::filesystem::exists(preproc_file_path)) {
        std::cout << "Note: Reusing existing preprocessed input file: " + 
                preproc_file_path << std::endl; 
        return;
    }
    
    std::ifstream ifs{input_file_path};
    if (!ifs) {
        throw std::invalid_argument(
                "Could not open input file for reading: " + input_file_path
        );
    }

    // Create preproc tmp dir if not exists.
    std::filesystem::create_directory(temp_data_dir_path);

    std::ofstream ofs{preproc_file_path};
    if (!ofs) {
        throw std::invalid_argument(
                "Could not open preprocessed input file for writing: " + 
                    preproc_file_path
        );
    }

    std::istream_iterator<std::string> ifs_it{ifs};
    std::istream_iterator<std::string> end_ifs_it{}; // EOS sentinel
    std::ostream_iterator<std::string> ofs_it{ofs, "\n"};

    std::transform(ifs_it, end_ifs_it, ofs_it, remove_punctuation);
}

std::string remove_punctuation(std::string s) {
    s.erase(
        std::remove_if(s.begin(), s.end(), ::ispunct),
        s.end()
    );

    return s;
}

}