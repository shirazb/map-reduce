// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <sb-mapreduce/worker.h>
#include <sb-mapreduce/common.h>

#include <memory>
#include <iterator>
#include <exception>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_set>

namespace shiraz::MapReduce {

class Master {
public:
    Master(
            std::shared_ptr<InputFilePaths> input_files,
            std::shared_ptr<OutputFilePaths> output_files,
            UserMapFunc map_f,
            UserReduceFunc reduce_f,
            int num_workers,
            IntermediateHashFunc intermediate_hash
    );

    // Not copyable.
    Master(const Master& m) =delete;
    Master& operator=(const Master& m) =delete;

    // Is Movable.
    Master(Master&& m) =default;
    Master& operator=(Master&& m) =default;

    void go();

    struct NotEnoughWorkersException;

private:
    std::shared_ptr<InputFilePaths> input_files; // size M
    std::shared_ptr<OutputFilePaths> output_files; // size R

    UserMapFunc map_f;
    UserReduceFunc reduce_f;

    int num_workers;

    IntermediateHashFunc intermediate_hash;

    std::vector<std::vector<std::string>>
    map_stage(
            std::unordered_set<Worker, Worker::Hash>& free_workers,
            std::unordered_set<Worker, Worker::Hash>& busy_workers
    );

    void
    reduce_stage(
            std::unordered_set<Worker, Worker::Hash>& free_workers,
            std::unordered_set<Worker, Worker::Hash>& busy_workers,
            const std::vector<std::vector<std::string>>& intermediate_file_paths
    );
};

struct Master::NotEnoughWorkersException: std::invalid_argument {
    NotEnoughWorkersException(
            std::size_t num_ifstreams,
            std::size_t num_ofstreams,
            int num_workers
    );

private:
    static
    std::string
    build_error_str(
            std::size_t num_ifstreams,
            std::size_t num_ofstreams,
            int num_workers
    );
};

} // namespace shiraz::MapReduce
