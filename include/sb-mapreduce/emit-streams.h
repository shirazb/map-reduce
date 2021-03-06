// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <sb-mapreduce/common.h>

#include <any>
#include <iterator>
#include <fstream>
#include <vector>
#include <utility>

namespace shiraz::MapReduce {

// using EmitResultStream = std::ofstream; Brought in by common.h

class EmitIntermediateStream;
class EmitIntermediateStreamIterator;

namespace test {
    class EmitIntermediateStream_T;
}

class EmitIntermediateStream {
public:
    EmitIntermediateStream(
            IntermediateHashFunc hash_inter,
            std::vector<std::ofstream> ofss
    );

    EmitIntermediateStreamIterator
    begin();

    /* Both lvalue and rvalue interface to stream functions. */

    template<typename K_i, typename V_i>
    EmitIntermediateStream&
    operator<<(IntermediateResult<K_i, V_i>& ir) {
        *this << std::move(ir);
        return *this;
    }

    template<typename K_i, typename V_i>
    EmitIntermediateStream&
    operator<<(IntermediateResult<K_i, V_i>&& ir) {
        const auto r = this->hash_inter(std::any{ir.first});
        this->ofss[r] << ir.first << "," << ir.second << std::endl;
        return *this;
    }

    template<typename K_i, typename V_i>
    void
    to_stream(IntermediateResult<K_i, V_i>& ir) {
        *this << ir;
    }

    template<typename K_i, typename V_i>
    void
    to_stream(IntermediateResult<K_i, V_i>&& ir) {
        *this << ir;
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

    friend class test::EmitIntermediateStream_T;
};

class EmitIntermediateStreamIterator {
public:
    using iterator_category = std::output_iterator_tag;

    /* Is default constructable, like std::ostream_iterator */
    EmitIntermediateStreamIterator() = default;

    explicit EmitIntermediateStreamIterator(
            EmitIntermediateStream *emit_s
    ) :
            emit_s{emit_s} {}

    /* Is copyable */

    EmitIntermediateStreamIterator(
            const EmitIntermediateStreamIterator&) = default;

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
