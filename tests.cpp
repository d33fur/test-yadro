#include "gtest/gtest.h"
#include <iostream>
#include <fstream>
#include <string>

class MyTestSuite : public ::testing::TestWithParam<int> {
protected:
    std::string get_test_file_name(int index) {
        return "./../test_files/input_tests/" + std::to_string(index) + ".txt";
    }

    std::string get_expected_output(int index) {
        std::ifstream expected_file("./../test_files/input_tests/expected.txt");
        std::string expected_output;
        for(int i = 1; i < index; i++) {
          std::getline(expected_file, expected_output);
        }
        std::getline(expected_file, expected_output);
        return expected_output;
    }

    std::string get_actual_output(const std::string& test_file_name) {
        std::string exe_path = "./task";
        std::string output_file = "output.txt";
        std::string command = exe_path + " " + test_file_name + " > " + output_file;
        int result = system(command.c_str());
        std::ifstream output(output_file);
        std::string actual_output;
        std::getline(output, actual_output);
        return actual_output;
    }
};

TEST_P(MyTestSuite, MyTestCase) {
    int index = GetParam();
    std::string test_file_name = get_test_file_name(index);
    std::string expected = get_expected_output(index);
    std::string actual = get_actual_output(test_file_name);
    EXPECT_EQ(expected, actual);
}

INSTANTIATE_TEST_SUITE_P(MyTestSuite, MyTestSuite, ::testing::Range(1, 40));

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
