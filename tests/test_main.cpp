#include <gtest/gtest.h>

TEST(SampleTest, AssertionTrue) {
    EXPECT_TRUE(true);
}

TEST(SampleTest, Addition) {
    EXPECT_EQ(1 + 1, 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}