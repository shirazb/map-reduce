// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/worker.h>

namespace shiraz::MapReduce {

void Worker::map_task(
        UserMapFunc map_f,
        InputFileIterator input_file_iterators
) {
    (void)map_f;
    (void)input_file_iterators;
}

} // namespace shiraz::MapReduce
