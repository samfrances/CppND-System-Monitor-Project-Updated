#include <cstdlib>
#include <string>

#include "gmock/gmock.h"

/**
 * Get the filesystem root used for these integration tests
 */
std::string GetTestFsRoot() {
    std::string TEST_FS_ROOT;
    if (const char* src = std::getenv("TEST_FS_ROOT")) {
        TEST_FS_ROOT = src;
    }
    return TEST_FS_ROOT;
}


TEST(Foo, OperatingSystem) {
  EXPECT_EQ(GetTestFsRoot(), "foo");
}
