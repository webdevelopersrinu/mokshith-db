enable_testing()

# Google Test - properly reference the third_party directory
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest ${CMAKE_BINARY_DIR}/googletest)
include_directories(${gtest_SOURCE_DIR}/include)

# For now, create a simple test file
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/simple_test.cpp "
#include <gtest/gtest.h>

TEST(BasicTest, SimpleAssertion) {
    EXPECT_EQ(1 + 1, 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
")

# Create test executable
add_executable(mokshith_test simple_test.cpp)
target_link_libraries(mokshith_test gtest gtest_main)

# Add test
add_test(NAME mokshith_test COMMAND mokshith_test)
