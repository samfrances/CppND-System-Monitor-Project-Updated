#include <cstdlib>
#include <string>

#include "gmock/gmock.h"

#include "linux_parser.h"

/**
 * Get the filesystem root used for these integration tests
 */
std::string GetTestFsRoot(std::string name) {
  std::string TEST_FS_ROOT;
  if (const char* src = std::getenv("TEST_FS_ROOT")) {
    TEST_FS_ROOT = src;
  }
  return TEST_FS_ROOT + name + "/";
}

TEST(LinuxParserIntegrationTest, OperatingSystem) {
  {
    LinuxParser parser(GetTestFsRoot("one"));

    EXPECT_EQ(parser.OperatingSystem(), "Fedora 17 (Beefy Miracle)");
  }

  {
    LinuxParser parser(GetTestFsRoot("two"));

    EXPECT_EQ(parser.OperatingSystem(), "Debian GNU/Linux jessie/sid");
  }
}