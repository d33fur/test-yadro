#include "gtest/gtest.h"
#include <iostream>
#include <fstream>
#include <string>

class Input_tests : public ::testing::TestWithParam<int> 
{
  protected:
    std::string get_test_file_name(int index, const std::string& path) 
    {
      return "./../test_files/" + path + "/" + std::to_string(index) + ".txt";
    }

    std::string get_expected_output(int index, const std::string& path) 
    {
      std::ifstream expected_file("./../test_files/" + path + "/expected.txt");
      std::string expected_output;
      for(int i = 1; i < index; i++) {
        std::getline(expected_file, expected_output);
      }
      std::getline(expected_file, expected_output);
      expected_file.close();
      return expected_output;
    }

    std::string get_actual_output(const std::string& test_file_name) 
    {
      std::string exe_path = "./task";
      std::string output_file = "output.txt";
      std::string command = exe_path + " " + test_file_name + " > " + output_file;
      int result = system(command.c_str());
      std::ifstream output(output_file);
      std::string actual_output;
      std::getline(output, actual_output);
      output.close();
      return actual_output;
    }
};

TEST_P(Input_tests, all_Tests) 
{
  int index = GetParam();
  std::string test_file_name = get_test_file_name(index, "input_tests");
  std::string expected = get_expected_output(index, "input_tests");
  std::string actual = get_actual_output(test_file_name);
  EXPECT_EQ(expected, actual);
}

INSTANTIATE_TEST_SUITE_P(T, Input_tests, ::testing::Range(1, 40));

class Unit_tests : public ::testing::TestWithParam<int> 
{
  protected:
    std::string get_test_file_name(int index) 
    {
      return "./../test_files/unit_tests/" + std::to_string(index) + ".txt";
    }

    std::string get_expected_file_name(int index) 
    {
      return "./../test_files/unit_tests/" +std::to_string(index) +"expected.txt";
    }

    int get_number_of_lines(const std::string& file_name)
    {
      std::ifstream file(file_name);
      int n = 0;
      std::string line;
      while (std::getline(file, line)) {
          ++n;
      }
      return n;
    }
};

TEST_P(Unit_tests, all_Tests) 
{
  int index = GetParam();
  std::string test_file_name = get_test_file_name(index);
  std::string expected_file_name = get_expected_file_name(index);

  std::string exe_path = "./task";
  std::string output_file = "output.txt";
  std::string command = exe_path + " " + test_file_name + " > " + output_file;
  int result = system(command.c_str());

  std::ifstream expected_output(expected_file_name);
  std::string expected, actual;
  std::ifstream test_output(output_file);

  EXPECT_EQ(get_number_of_lines(expected_file_name), get_number_of_lines(output_file));

  while(std::getline(expected_output, expected) && std::getline(test_output, actual))
  {
    EXPECT_EQ(expected, actual);
  }
  
  expected_output.close();
  test_output.close();
}

INSTANTIATE_TEST_SUITE_P(T, Unit_tests, ::testing::Range(1, 10));


int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
