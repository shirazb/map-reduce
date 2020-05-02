// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/master.h>

namespace shiraz::MapReduce {

Master::Master(
        InputFileIterator input_file_iterators,
        OutputFileIterator output_file_iterators,
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

}

void Master::go() {

}

} // shiraz::MapReduce
