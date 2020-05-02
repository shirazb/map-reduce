// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/master.h>

namespace shiraz::MapReduce {

Master::Master(
        std::vector<std::istream_iterator<std::string>> input_files,
        std::vector<std::ostream_iterator<std::string>> output_files,
        UserMapFunc map_f,
        int num_workers,
        IntermediateHashFunc intermediate_hash
) :
        input_files{std::move(input_files)},
        output_files{std::move(output_files)},
        map_f{map_f},
        num_workers{num_workers},
        intermediate_hash{intermediate_hash}
{

}

void Master::go() {

}

} // shiraz::MapReduce
