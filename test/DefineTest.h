//
// Created by Zhongwen Wang on 2023/12/14.
//

#ifndef BEANDEFINETEST_H
#define BEANDEFINETEST_H

#include "gtest/gtest.h"

class DefineTest : public ::testing::Test {
  public:
    static void SetUpTestSuite() {}

    static void TearDownTestSuite() {}

  protected:
    void SetUp() override;

    void TearDown() override;
};

#endif  // BEANDEFINETEST_H
