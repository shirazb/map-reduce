// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/master.h>
#include <sb-mapreduce/worker.h>

#include <unordered_set>

namespace shiraz::MapReduce {

Master::Master(
        std::vector<InputFileIterator> input_file_iterators,
        std::vector<OutputFileIterator> output_file_iterators,
        UserMapFunc map_f,
        int num_workers,
        IntermediateHashFunc intermediate_hash
) :
        input_file_iterators{std::move(input_file_iterators)},
        output_file_iterators{std::move(output_file_iterators)},
        map_f{map_f},
        num_workers{num_workers},
        intermediate_hash{intermediate_hash}
{
    // Establish invariant: Enough workers for the tasks.
    if (input_file_iterators.size() > num_workers ||
            output_file_iterators.size() > num_workers) {
        throw MasterNotEnoughWorkersException(
                input_file_iterators.size(),
                output_file_iterators.size(),
                num_workers
        );
    }
}

void Master::go() {
    std::unordered_set<Worker, Worker::Hash> free_workers;
    std::unordered_set<Worker, Worker::Hash> busy_workers;

    // Construct all the workers.
    for (int i = 0; i < this->num_workers; i++) {
        free_workers.emplace(Worker{i});
    }

    // Iterator of file stream iterators
    auto cur_ifstream_it_it = this->input_file_iterators.begin();
    const auto end_ifstream_it_it = this->input_file_iterators.end();

    // Assume for now we definitely have enough workers to do this in iteration
    // of the outer loop.

    // While files remain, schedule as many map tasks as possible.
    while (cur_ifstream_it_it != end_ifstream_it_it) {
        while (!free_workers.empty()) {
            Worker w{*free_workers.begin()};
            w.map_task(this->map_f, *cur_ifstream_it_it);

            busy_workers.emplace(std::move(w));

            ++cur_ifstream_it_it;
        }
    }

    // This would happen from messages passed from worker to master.
    std::swap(free_workers, busy_workers);
}

} // namespace shiraz::MapReduce
