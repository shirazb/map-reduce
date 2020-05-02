// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include "master.h"

namespace shiraz::MapReduce {

class Worker {
public:
    void map_task(UserMapFunc map_f, InputFileIterator input_file_iterators);
};

} // namespace shiraz::MapReduce
