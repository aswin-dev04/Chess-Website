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

#define ASSERT_LT(expected, actual)                                            \
  if (!((actual) < (expected))) {                                              \
    std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ << " Expected "       \
              << (actual) << " to be less than " << (expected) << std::endl;   \
    return false;                                                              \
  }

#define ASSERT_GT(expected, actual)                                            \
  if (!((actual) > (expected))) {                                              \
    std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ << " Expected "       \
              << (actual) << " to be greater than " << (expected)              \
              << std::endl;                                                    \
    return false;                                                              \
  }

#define ASSERT_LE(expected, actual)                                            \
  if (!((actual) <= (expected))) {                                             \
    std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ << " Expected "       \
              << (actual) << " to be less than or equal to " << (expected)     \
              << std::endl;                                                    \
    return false;                                                              \
  }

#define ASSERT_GE(expected, actual)                                            \
  if (!((actual) >= (expected))) {                                             \
    std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ << " Expected "       \
              << (actual) << " to be greater than or equal to " << (expected)  \
              << std::endl;                                                    \
    return false;                                                              \
  }

#define ASSERT_NE(expected, actual)                                            \
  if ((actual) == (expected)) {                                                \
    std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ << " Expected "       \
              << (actual) << " to not equal " << (expected) << std::endl;      \
    return false;                                                              \
  }
#endif
