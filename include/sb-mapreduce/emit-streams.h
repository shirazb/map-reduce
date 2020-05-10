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
#include <sstream>

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

    TEST_CASE_CLASS("[libsb-mapreduce] EmitIntermediateStream") {
        /* setup */

        IntermediateHashFunc hf = [](std::any x) -> std::size_t {
            return static_cast<std::size_t>(std::any_cast<int>(x));
        };
        std::vector<std::ofstream> ofss;

        // Dummy test file.
        auto test_file_prefix = std::filesystem::temp_directory_path();
        test_file_prefix /= "sb-mapreduce-EmitIntermediateStream-constructor-test-";
        std::vector<decltype(test_file_prefix)> test_fps;

        // Dummy test results
        std::vector<IntermediateResult<int, std::string>> irs;
        std::string test_word_prefix = "dfjk34df-";

        constexpr int num_ofs = 5;

        for (int i = 0; i < num_ofs; i++) {
            // Store the fp.
            const auto& fp = test_file_prefix.string() + std::to_string(i);
            test_fps.emplace_back(fp);

            // Store the ofs.
            ofss.emplace_back(std::ofstream{fp});

            IntermediateResult<int, std::string> ir{i, test_word_prefix
                    + std::to_string(i)};

            // Store the IR.
            irs.emplace_back(std::move(ir));
        }

        // Construct.
        EmitIntermediateStream emit{hf, std::move(ofss)};

        SUBCASE("Constructor sets fields successfully and without modification") {
            CHECK(emit.hash_inter == hf);
            CHECK(emit.ofss.size() == num_ofs);

            for (int i = 0; i < num_ofs; i++) {
                // Write test data directly to ofs (we are not testing << of emit).
                emit.ofss[i] << irs[i].second;

                emit.ofss[i].close();
                std::ifstream ifs{test_fps[i]};

                std::string actual_word;
                ifs >> actual_word;

                CHECK(ifs.eof());
                CHECK(actual_word == irs[i].second);
            }
        }

        SUBCASE("Each emit stream interface correctly streams only the IR to only the correct file") {

            using stream_type = void(EmitIntermediateStream&, IntermediateResult<int>&);

            const auto stream1 = [](EmitIntermediateStream& emit, IntermediateResult<int>& ir) {
                emit << ir;
            };
            const auto stream2 = [](EmitIntermediateStream& emit, IntermediateResult<int>& ir) {
                auto ir_copy = ir;
                emit << std::move(ir_copy);
            };
            const auto stream3 = [](EmitIntermediateStream& emit, IntermediateResult<int>& ir) {
                emit.to_stream(ir);
            };
            const auto stream4 = [](EmitIntermediateStream& emit, IntermediateResult<int>& ir) {
                auto ir_copy = ir;
                emit.to_stream(std::move(ir_copy));
            };

            std::vector<std::function<stream_type>> stream_fs;
            stream_fs.emplace_back(stream1);
            stream_fs.emplace_back(stream2);
            stream_fs.emplace_back(stream3);
            stream_fs.emplace_back(stream4);

            const char *stream_strs[] = {
                    "(<< &)", "(<< &&)",
                    "to_stream(&)", "to_stream(&&)"
            };

            // For each stream_f, run the test. Recall the entire setup is redone
            // for each subcase.
            for (int j = 0; j < stream_fs.size(); j++) {
                SUBCASE(stream_strs[j]) {
                    for (int i = 0; i < num_ofs; i++) {
                        auto& ir = irs[i];

                        // Perform the test.
                        stream_fs[j](emit, ir);

                        std::ostringstream expected;
                        expected << ir.first << "," << ir.second;

                        // NB: Will break if we introduce whitespace around comma in `k,v`.
                        std::ifstream ifs{test_fps[ir.first]};
                        std::string actual;
                        ifs >> actual;

                        // Check next character is endl.
                        // Note getline() will not put the delim char (endl) into next.
                        std::string next;
                        std::getline(ifs, next);
                        CHECK(next.empty());

                        // Read again, check hit EOF.
                        ifs >> next;
                        CHECK(ifs.eof());

                        // Check the IR was outputted correctly to the correct ofstream.
                        CHECK(expected.str() == actual);
                    }
                }
            }
        }

        /* teardown */

        for (int i = 0; i < num_ofs; i++) {
            std::filesystem::remove(test_fps[i]);
        }
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
