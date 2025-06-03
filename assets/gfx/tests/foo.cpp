#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::ThrowsMessage;


namespace {
TEST(FooTest, Check) { EXPECT_EQ(0, (int)(0 + 0)); }
}  // namespace
