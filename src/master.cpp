// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/master.h>
#include <sb-mapreduce/worker.h>
#include <sb-mapreduce/common.h>

#include <unordered_set>
#include <sstream>
#include <iostream>

namespace shiraz::MapReduce {

Master::Master(
        std::vector<InputFileIterator> input_file_iterators,
        std::vector<OutputFileIterator> output_file_iterators,
        UserMapFunc map_f,
        UserReduceFunc reduce_f,
        int num_workers,
        IntermediateHashFunc intermediate_hash
) :
        input_file_iterators{std::move(input_file_iterators)},
        output_file_iterators{std::move(output_file_iterators)},
        map_f{map_f},
        reduce_f{reduce_f},
        num_workers{num_workers},
        intermediate_hash{intermediate_hash}
{
    // Establish invariant: Enough workers for the tasks.
    if (this->input_file_iterators.size() > this->num_workers ||
            this->output_file_iterators.size() > this->num_workers) {
        throw Master::NotEnoughWorkersException(
                this->input_file_iterators.size(),
                this->output_file_iterators.size(),
                this->num_workers
        );
    }
}

void Master::go() {
    std::unordered_set<Worker, Worker::Hash> free_workers;
    std::unordered_set<Worker, Worker::Hash> busy_workers;

    /* Construct all the workers. */

    for (int i = 0; i < this->num_workers; i++) {
        free_workers.emplace(Worker{i});
    }

    const auto intermediate_file_paths = this->map_stage(
            free_workers, busy_workers
    );

    /* Reduce Stage */

    int cur_output_file_idx = 0;
    auto cur_output_file_it = this->output_file_iterators.begin();
    auto output_file_end_it = this->output_file_iterators.end();

    while (cur_output_file_it != output_file_end_it) {
        while (!free_workers.empty() && cur_output_file_it != output_file_end_it) {
            Worker w = free_workers.extract(
                    free_workers.begin()
            ).value();

            // TODO: For now, we just take 1 intermediate file instead of R.
            std::ifstream inter_ifs{intermediate_file_paths[cur_output_file_idx]};
            std::istream_iterator<std::string> inter_it{inter_ifs};

            w.reduce_task(reduce_f, inter_ifs, *cur_output_file_it);

            busy_workers.emplace(std::move(w));

            ++cur_output_file_it;
            ++cur_output_file_idx;
        }
    }

    free_workers.merge(busy_workers);

    /* Cleanup */

    // TODO: RAII-ify the intermediate file resource. Also, consider who should 
    // delete / have ownership.

    std::for_each(intermediate_file_paths.begin(), intermediate_file_paths.end(),
            [](auto& fp) { std::filesystem::remove(fp); }
    );
}

/**
 * Invariant: All workers are free before and after this function.
 * Return the vector of intermediate file paths produced by the map tasks.
 */
std::vector<std::string>
Master::map_stage(
        std::unordered_set<Worker, Worker::Hash>& free_workers,
        std::unordered_set<Worker, Worker::Hash>& busy_workers
) {
    /* Store intermediate output paths returned by map so we can forward to
       reduce */
    
    std::vector<std::string> intermediate_file_paths;

    /* Map Stage */

    // Iterator of file stream iterators
    auto cur_ifstream_it_it = this->input_file_iterators.begin();
    const auto end_ifstream_it_it = this->input_file_iterators.end();

    // Assume for now we definitely have enough workers to do this in iteration
    // of the outer loop.

    // While workers remain, keep scheduling tasks.
    // Move free workers into busy set.
    // Invoke map task on free worker and store returned intermediate file path.
    while (cur_ifstream_it_it != end_ifstream_it_it) {
        while (!free_workers.empty() && cur_ifstream_it_it != end_ifstream_it_it) {
            Worker w = free_workers.extract(
                    free_workers.begin()
            ).value();

            intermediate_file_paths.emplace_back(
                    w.map_task(
                        this->map_f, *cur_ifstream_it_it
                    )
            );

            busy_workers.emplace(std::move(w));

            ++cur_ifstream_it_it;
        }
    }

    // Extract all workers from busy to free set.
    // This would happen from messages passed from worker to master.
    free_workers.merge(busy_workers);

    return intermediate_file_paths;
}

Master::NotEnoughWorkersException::NotEnoughWorkersException(
            std::size_t num_ifstreams,
            std::size_t num_ofstreams,
            int num_workers
    ):
            invalid_argument(build_error_str(
                    num_ifstreams, num_ofstreams, num_workers
            ))
    {}

std::string Master::NotEnoughWorkersException::build_error_str(
            std::size_t num_ifstreams,
            std::size_t num_ofstreams,
            int num_workers
    ) {
        std::ostringstream msg;
        msg << "Number workers must be at least the number of ifstreams and " <<
            "number of ofstreams. " <<
            "Number workers = " << num_workers <<
            ". Number ifstreams = " << num_ifstreams <<
            ". Number ofstreams = " << num_ofstreams;

        return msg.str();
    }

} // namespace shiraz::MapReduce
