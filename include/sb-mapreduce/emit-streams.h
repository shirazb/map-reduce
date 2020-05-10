// Maintainer: Shiraz Butt (shiraz.b@icloud.com).
#pragma once

#include <sb-mapreduce/common.h>

#include <any>
#include <iterator>
#include <fstream>
#include <vector>
#include <utility>

#include <doctest/doctest.h>
#include <filesystem>

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

    /* Both lvalue and rvalue interface to stream functions. */

    template<typename K_i, typename V_i>
    EmitIntermediateStream&
    operator<<(IntermediateResult<K_i, V_i>& ir) {
        this << std::move(ir);
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
        this << ir;
    }

    template<typename K_i, typename V_i>
    void
    to_stream(IntermediateResult<K_i, V_i>&& ir) {
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

    TEST_CASE_CLASS("[libsb-mapreduce] EmitIntermediateStream constructor sets "
                    "its fields successfully and without modification") {
        IntermediateHashFunc hf = [](std::any x) -> std::size_t {
            return std::any_cast<std::size_t>(x);
        };
        std::vector<std::ofstream> ofss;

        // Dummy test file.
        auto test_file = std::filesystem::temp_directory_path();
        test_file /= "sb-mapreduce-EmitIntermediateStream-constructor-test";

        // Dummy ofs containing dummy word.
        std::ofstream ofs{test_file};
        std::string test_word = "234sdfdsf";
        ofs << test_word;


        // Construct.
        ofss.emplace_back(std::move(ofs));
        EmitIntermediateStream emit{hf, std::move(ofss)};

        // Check fields.

        CHECK(emit.hash_inter == hf);
        CHECK(emit.ofss.size() == 1);

        emit.ofss[0].close();
        std::ifstream ifs{test_file};

        std::string actual_word;
        ifs >> actual_word;

        CHECK(ifs.eof());
        CHECK(actual_word == test_word);

        // Delete test file.
        std::filesystem::remove(test_file);
    }
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
