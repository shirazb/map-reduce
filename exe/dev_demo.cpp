// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/version.h>
#include <sb-mapreduce/master.h>
#include <sb-mapreduce/common.h>

#include <memory>
#include <string>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <sstream>

using namespace shiraz;

#define NUM_WORKERS 10

namespace {

/*********************** file paths *******************************************/

// CWD must be map-reduce root in build dir.

using ConStr = const std::string;

ConStr data_root_path = "data/";

ConStr input_data_dir_path = data_root_path + "inputs/";
ConStr input_file_name = "pg-being_ernest.txt";
ConStr input_file_path = input_data_dir_path + input_file_name;

ConStr temp_data_dir_path = data_root_path + "tmp/";
ConStr preproc_files_prefix = "preprocessed_words_only_";

ConStr preproc_file_path = temp_data_dir_path + preproc_files_prefix
        + input_file_name;

ConStr output_data_dir_path = data_root_path + "outputs/";
ConStr output_file_name = "word_count.txt";
ConStr output_file_path = output_data_dir_path + output_file_name;

/*********************** prototypes *******************************************/

void
run_mapreduce_job();

void
preprocess_input_file(
        const std::string input_file_path,
        const std::string preproc_file_path
);

std::string
remove_punctuation(std::string s);

void
map_f(std::ifstream& ifs, MapReduce::IntermediateEmitter& emit);

const auto intermediate_hash = [](int k){ return k % NUM_WORKERS; };

void
reduce_f(
        std::string ikey,
        std::list<std::string> ivalues,
        MapReduce::ResultEmitter& emit
);

template<typename T>
std::shared_ptr<T> make_shared_ptr_to_stack(T *e);

}

/*********************** main() ***********************************************/

int main() {
    std::cout << "sb-mapreduce" << std::endl;
    std::cout << "Version: " << SB_MAPREDUCE_VERSION << std::endl;
    std::cout << std::endl;

    preprocess_input_file(input_file_path, preproc_file_path);
    std::cout << std::endl;

    run_mapreduce_job();

    std::cout << std::endl;
    MapReduce::utils::log_file(output_file_path);
}

/*********************** helpers **********************************************/

namespace {

void
run_mapreduce_job() {
    std::ifstream preproc_ifs{preproc_file_path};
    MapReduce::InputFileStreams inputs;
    inputs.emplace_back(std::move(preproc_ifs));

    std::ofstream output_ofs{output_file_path, 
            std::ofstream::out | std::ofstream::trunc
    };
    MapReduce::OutputFileStreams outputs;
    outputs.emplace_back(std::move(output_ofs));
    
    // Construct shared_ptr to stack variables with dummy "deleter"
    MapReduce::Master master{
            make_shared_ptr_to_stack<MapReduce::InputFileStreams>(&inputs),
            make_shared_ptr_to_stack<MapReduce::OutputFileStreams>(&outputs),
            map_f, reduce_f,
            NUM_WORKERS,
            intermediate_hash
    };

    master.go();
}

void
preprocess_input_file(
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

std::string
remove_punctuation(std::string s) {
    s.erase(
        std::remove_if(s.begin(), s.end(), ::ispunct),
        s.end()
    );

    return s;
}

void
map_f(std::ifstream& ifs, MapReduce::IntermediateEmitter& emit) {
    for (std::string s; ifs >> s; ) {
        emit(s, std::to_string(1));
    }
}

void
reduce_f(
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
    res << "(" << ikey << "," << acc << ")";    

    emit(res.str());
}

template<typename T>
std::shared_ptr<T> make_shared_ptr_to_stack(T *e) {
    // Use dummy deleter that does nothing
    return std::shared_ptr<T>{e, [](auto){}};
}

}