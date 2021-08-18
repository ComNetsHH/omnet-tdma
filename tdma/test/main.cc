#define UNIT_TEST

// access all private/protected members for testing purposes
#define private public
#define protected public

// include google testing lib
#include <iostream>
#include <gtest/gtest.h>

// Test suites
#include "rlcCoreTest.cc"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}