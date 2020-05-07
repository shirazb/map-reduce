// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/emit-streams.h>

#include <algorithm>
#include <vector>
#include <fstream>

namespace shiraz::MapReduce {

EmitIntermediateStream::EmitIntermediateStream(
        IntermediateHashFunc hash_inter,
        std::vector<std::ofstream> ofss
):
        hash_inter{hash_inter},
        ofss{std::move(ofss)} {}

bool
EmitIntermediateStream::operator!() const {
    return std::all_of(
            this->ofss.begin(), this->ofss.end(),
            [](auto& ofs) { return !ofs; }
    );
}

EmitIntermediateStream::operator bool() const {
    return std::all_of(
            this->ofss.begin(), this->ofss.end(),
            [](auto& ofs) { return (bool) ofs; }
    );
}

/**
 * For convenience, and to hide type of output iterator created. In
 * particular, hide the delimiter parameter passed to constructor.
 */
EmitIntermediateStreamIterator
EmitIntermediateStream::begin() {
    return EmitIntermediateStreamIterator{this};
}

} // namespace shiraz::MapReduce
