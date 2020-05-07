// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <sb-mapreduce/common.h>

#include <iterator>
#include <fstream>
#include <vector>
#include <utility>

namespace shiraz::MapReduce {

// using EmitResultStream = std::ofstream; Brought in by common.h

class EmitIntermediateStream;
class EmitIntermediateStreamIterator;

class EmitIntermediateStream {
public:
    EmitIntermediateStream(
            IntermediateHashFunc hash_inter,
            std::vector<std::ofstream> ofss
    );

    EmitIntermediateStreamIterator
    begin();

    template<typename K_i, typename V_i>
    EmitIntermediateStream&
    operator<<(IntermediateResult<K_i, V_i>& ir) {
        const int r = this->hash_inter(ir.first);
        this->ofss[r] << ir.first << "," << ir.second << std::endl;
        return *this;
    }

    template<typename K_i, typename V_i>
    void
    to_stream(IntermediateResult<K_i, V_i>& ir) {
        this << ir;
    }

    /* Composite boolean operators that `all` recurse on each sub-ofs. */

    bool
    operator!() const;

    explicit
    operator bool() const;

    // Is movable, not copyable. Copy already deleted in superclass.
    EmitIntermediateStream(EmitIntermediateStream&&) = default;

    EmitIntermediateStream& operator=(EmitIntermediateStream&&) = default;

private:
    IntermediateHashFunc hash_inter;
    std::vector<std::ofstream> ofss;
};

class EmitIntermediateStreamIterator {
public:
    using iterator_category = std::output_iterator_tag;

    EmitIntermediateStreamIterator() = default;

    explicit EmitIntermediateStreamIterator(
            EmitIntermediateStream *emit_s
    ) :
            emit_s{emit_s} {}

    /* Is default constructable, like std::ostream_iterator */

    EmitIntermediateStreamIterator(
            const EmitIntermediateStreamIterator&) = default;

    /* Is copyable */

    EmitIntermediateStreamIterator&
    operator=(const EmitIntermediateStreamIterator&) = default;

    /* Is movable */

    EmitIntermediateStreamIterator(EmitIntermediateStreamIterator&&) = default;

    EmitIntermediateStreamIterator&
    operator=(EmitIntermediateStreamIterator&&) = default;

    /* Operator no-ops to allow the idiomatic iterator syntax */

    // No-op
    EmitIntermediateStreamIterator&
    operator++() { return *this; }

    // No-op
    EmitIntermediateStreamIterator&
    operator++(int) { return *this; }

    // No-op
    EmitIntermediateStreamIterator&
    operator*() { return *this; }

    /**
     * Write to underlying output stream.
     */
    template<typename K_i, typename V_i>
    EmitIntermediateStreamIterator&
    operator=(const IntermediateResult<K_i, V_i>& ir) {
        emit_s << ir;
        return *this;
    }

private:
    // In LLVM, they use a raw pointer to make this default constructable.
    EmitIntermediateStream *emit_s;
};

} // namespace shiraz::MapReduce
