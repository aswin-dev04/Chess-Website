#ifndef TEST_HPP
#define TEST_HPP
#include <iostream>
#include <string>

#define ASSERT_EQ(expected, actual)                                            \
  if ((expected) != (actual)) {                                                \
    std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ << " Expected "       \
              << (expected) << " but got " << (actual) << std::endl;           \
    return false;                                                              \
  }

#define ASSERT_TRUE(condition)                                                 \
  if (!(condition)) {                                                          \
    std::cout << "FAIL: " << __FILE__ << ":" << __LINE__                       \
              << " Expected true but got false" << std::endl;                  \
    return false;                                                              \
  }

#define RUN_TEST(test_func)                                                    \
  if (test_func()) {                                                           \
    std::cout << "PASS: " << #test_func << std::endl;                          \
  } else {                                                                     \
    std::cout << "FAIL: " << #test_func << std::endl;                          \
  }

#endif
