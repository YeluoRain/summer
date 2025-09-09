#ifndef TEST_INJECTTEST
#define TEST_INJECTTEST

#include "gtest/gtest.h"

class InjectTest : public ::testing::Test {
public:
  static void SetUpTestSuite() {}

  static void TearDownTestSuite() {}

protected:
  void SetUp() override{};

  void TearDown() override{};
};

#endif /* TEST_INJECTTEST */
