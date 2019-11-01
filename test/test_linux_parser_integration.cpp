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

TEST(LinuxParserIntegrationTest, Kernel) {
  {
    LinuxParser parser(GetTestFsRoot("one"));

    EXPECT_EQ(parser.Kernel(), "2.6.18-92.el5");
  }

  {
    LinuxParser parser(GetTestFsRoot("two"));

    EXPECT_EQ(parser.Kernel(), "3.6.8-1.523");
  }
}

TEST(LinuxParserIntegrationTest, Pids) {
  {
    LinuxParser parser(GetTestFsRoot("one"));

    auto actual_pids = parser.Pids();
    std::sort(actual_pids.begin(), actual_pids.end());

    std::vector<int> expected_pids{3, 83, 490, 1259, 8193};
    std::sort(expected_pids.begin(), expected_pids.end());

    EXPECT_THAT(actual_pids, ::testing::ContainerEq(expected_pids));
  }

  {
    LinuxParser parser(GetTestFsRoot("two"));

    auto actual_pids = parser.Pids();
    std::sort(actual_pids.begin(), actual_pids.end());

    std::vector<int> expected_pids{928, 9, 4482, 8840, 820, 3, 5, 7, 93928};
    std::sort(expected_pids.begin(), expected_pids.end());

    EXPECT_THAT(actual_pids, ::testing::ContainerEq(expected_pids));
  }
}