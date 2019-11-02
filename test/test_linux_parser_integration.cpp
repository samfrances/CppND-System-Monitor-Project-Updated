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

TEST(LinuxParserIntegrationTest, MemoryUtilization) {
  {
    LinuxParser parser(GetTestFsRoot("one"));

    float expected_memfree = 1374408;
    float expected_memtotal = 1921988;
    float expected_utilization =
        (expected_memtotal - expected_memfree) / expected_memtotal;

    EXPECT_NEAR(parser.MemoryUtilization(), expected_utilization, 0.01);
  }

  {
    LinuxParser parser(GetTestFsRoot("two"));

    float expected_memfree = 431064;
    float expected_memtotal = 3973736;
    float expected_utilization =
        (expected_memtotal - expected_memfree) / expected_memtotal;

    EXPECT_NEAR(parser.MemoryUtilization(), expected_utilization, 0.01);
  }
}

TEST(LinuxParserIntegrationTest, UpTime) {
  {
    LinuxParser parser(GetTestFsRoot("one"));

    long expected = 23974;

    EXPECT_EQ(parser.UpTime(), expected);
  }

  {
    LinuxParser parser(GetTestFsRoot("two"));

    long expected = 13145;

    EXPECT_EQ(parser.UpTime(), expected);
  }
}

TEST(LinuxParserIntegrationTest, ActiveJiffies_Pid) {
  LinuxParser parser(GetTestFsRoot("one"));

  long expected = 582 + 205;
  EXPECT_EQ(parser.ActiveJiffies(490), expected);
}

TEST(LinuxParserIntegrationTest, CpuUtilization) {
  {
    LinuxParser parser(GetTestFsRoot("one"));

    auto usage = parser.CpuUtilization();

    EXPECT_EQ(usage.user, 209841);
    EXPECT_EQ(usage.nice, 1554);
    EXPECT_EQ(usage.system, 21720);
    EXPECT_EQ(usage.idle, 118519346);
    EXPECT_EQ(usage.iowait, 72939);
    EXPECT_EQ(usage.irq, 154);
    EXPECT_EQ(usage.softirq, 27168);
    EXPECT_EQ(usage.steal, 12);
  }

  {
    LinuxParser parser(GetTestFsRoot("two"));

    auto usage = parser.CpuUtilization();

    EXPECT_EQ(usage.user, 57226);
    EXPECT_EQ(usage.nice, 1340);
    EXPECT_EQ(usage.system, 12610);
    EXPECT_EQ(usage.idle, 1319920);
    EXPECT_EQ(usage.iowait, 1981);
    EXPECT_EQ(usage.irq, 0);
    EXPECT_EQ(usage.softirq, 864);
    EXPECT_EQ(usage.steal, 0);
  }
}

TEST(LinuxParserIntegrationTest, TotalProcesses) {
  {
    LinuxParser parser(GetTestFsRoot("one"));

    EXPECT_EQ(parser.TotalProcesses(), 21905);
  }

  {
    LinuxParser parser(GetTestFsRoot("two"));

    EXPECT_EQ(parser.TotalProcesses(), 8483);
  }
}

TEST(LinuxParserIntegrationTest, RunningProcesses) {
  {
    LinuxParser parser(GetTestFsRoot("one"));

    EXPECT_EQ(parser.RunningProcesses(), 1);
  }

  {
    LinuxParser parser(GetTestFsRoot("two"));

    EXPECT_EQ(parser.RunningProcesses(), 15);
  }
}