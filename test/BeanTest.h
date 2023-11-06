//
// Created by Zhongwen Wang on 2023/11/6.
//

#ifndef CCTEST_BEANTEST_H
#define CCTEST_BEANTEST_H

#include "gtest/gtest.h"

class BeanTest : public ::testing::Test{
public:
    static void SetUpTestSuite() {}

    static void TearDownTestSuite() {}
protected:
    void SetUp() override;

    void TearDown() override;
};


#endif//CCTEST_BEANTEST_H
