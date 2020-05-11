// Maintainer:  (shiraz.b@icloud.com).

#include <sb-mapreduce/common.h>

#include <doctest/doctest.h>

#include <filesystem>
#include <iterator>
#include <iostream>
#include <string>
#include <sstream>

namespace shiraz::MapReduce::test {

TEST_CASE("[libsb-mapreduce] log_file() throws on malformed filepath") {
    std::string not_exists = "dfgjk345#$%";

    CHECK_THROWS(utils::log_file(not_exists));
}

TEST_CASE("[libsb-mapreduce] log_file() prints lines correctly") {
    /* setup */

    // Dummy test file.
    auto test_fp = std::filesystem::temp_directory_path();
    test_fp /= "sb-mapreduce-common-test-log_file-correctness";

    constexpr int num_lines = 40;

    // Write lines to test file.
    {
        std::ofstream ofs{test_fp};
        REQUIRE(ofs);
        for (int i = 0; i < num_lines; i++) {
            ofs << i << std::endl;
        }
    }

    /* test */

    std::stringstream dummy_cout;
    utils::log_file(test_fp, num_lines, '\n', dummy_cout);

    dummy_cout.seekp(0, std::ios_base::beg);

    for (int i = 0; i < num_lines; i++) {
        std::string actual;
        dummy_cout >> actual;
        auto expected = std::to_string(i);

        REQUIRE(actual == expected); // no point doing more on failure
    }

    // Consume one more time to go past the end.
    REQUIRE(!dummy_cout.eof());
    {
        std::string s;
        dummy_cout >> s;
    }
    REQUIRE(dummy_cout.eof());

    /* teardown */

    // Restore original std::cout buffer and delete test file.
    std::filesystem::remove(test_fp);
}

} // namespace shiraz::MapReduce::test
