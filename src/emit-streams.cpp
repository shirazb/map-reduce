// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#include <sb-mapreduce/emit-streams.h>

#include <algorithm>
#include <vector>

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

/*
#include <doctest/doctest.h>

#include <any>

using namespace shiraz::MapReduce;

TEST_CASE_CLASS("EmitIntermediateStream constructor sets its fields") {
    IntermediateHashFunc hf = [](std::any x) {
            return std::any_cast<std::size_t>(x);
    };

    std::vector<std::ofstream> ofss;

    EmitIntermediateStream emit{hf, ofss};

    CHECK(emit.hash_inter == hf);
    CHECK(emit.ofss == ofss);
}
*/
