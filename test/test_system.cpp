#include "gmock/gmock.h"

#include <vector>

#include "linux_parser.h"
#include "linux_parser_pure.h"
#include "system.h"

using ::testing::Return;

class MockParser : public ILinuxParser {
 public:
  // System
  MOCK_METHOD(float, MemoryUtilization, (), (const, override));
  MOCK_METHOD(long, UpTime, (), (const, override));
  MOCK_METHOD(std::vector<int>, Pids, (), (const, override));
  MOCK_METHOD(int, TotalProcesses, (), (const, override));
  MOCK_METHOD(int, RunningProcesses, (), (const, override));
  MOCK_METHOD(std::string, OperatingSystem, (), (const, override));
  MOCK_METHOD(std::string, Kernel, (), (const, override));

  // CPU
  MOCK_METHOD(LinuxParserPure::CpuUtilizationSnapshot, CpuUtilization, (),
              (const, override));

  // Processes
  MOCK_METHOD(std::string, Command, (int pid), (const, override));
  MOCK_METHOD(std::string, Ram, (int pid), (const, override));
  MOCK_METHOD(std::string, Uid, (int pid), (const, override));
  MOCK_METHOD(std::string, User, (int pid), (const, override));
  MOCK_METHOD(long int, UpTime, (int pid), (const, override));
  MOCK_METHOD(long, ActiveJiffies, (int pid), (const, override));
};

TEST(System, Kernel) {
  MockParser parser;
  System system(parser);

  std::vector<std::string> expectations{"foo", "bar", "", "blahblah"};

  for (const auto expected : expectations) {
    ON_CALL(parser, Kernel()).WillByDefault(Return(expected));

    EXPECT_CALL(parser, Kernel()).Times(1);

    EXPECT_EQ(system.Kernel(), expected);
  }
}

TEST(System, MemoryUtilization) {
  MockParser parser;
  System system(parser);

  std::vector<float> expectations{0.72, 0.1, 1.0, 0};

  for (const float expected : expectations) {
    ON_CALL(parser, MemoryUtilization()).WillByDefault(Return(expected));

    EXPECT_CALL(parser, MemoryUtilization()).Times(1);

    EXPECT_EQ(system.MemoryUtilization(), expected);
  }
}

TEST(System, OperatingSystem) {
  MockParser parser;
  System system(parser);

  std::vector<std::string> expectations{"foo", "bar", "", "blahblah"};

  for (const auto expected : expectations) {
    ON_CALL(parser, OperatingSystem()).WillByDefault(Return(expected));

    EXPECT_CALL(parser, OperatingSystem()).Times(1);

    EXPECT_EQ(system.OperatingSystem(), expected);
  }
}

TEST(System, RunningProcesses) {
  MockParser parser;
  System system(parser);

  std::vector<int> expectations{938475, 4, 974, 9, 0, 2184, 33};

  for (const float expected : expectations) {
    ON_CALL(parser, RunningProcesses()).WillByDefault(Return(expected));

    EXPECT_CALL(parser, RunningProcesses()).Times(1);

    EXPECT_EQ(system.RunningProcesses(), expected);
  }
}

TEST(System, TotalProcesses) {
  MockParser parser;
  System system(parser);

  std::vector<int> expectations{938475, 4, 974, 9, 17, 0, 2184, 33};

  for (const float expected : expectations) {
    ON_CALL(parser, TotalProcesses()).WillByDefault(Return(expected));

    EXPECT_CALL(parser, TotalProcesses()).Times(1);

    EXPECT_EQ(system.TotalProcesses(), expected);
  }
}

TEST(System, UpTime) {
  MockParser parser;
  System system(parser);

  std::vector<long> expectations{238475, 4, 974, 99, 17, 0, 2184, 33};

  for (const float expected : expectations) {
    ON_CALL(parser, UpTime()).WillByDefault(Return(expected));

    EXPECT_CALL(parser, UpTime()).Times(1);

    EXPECT_EQ(system.UpTime(), expected);
  }
}

TEST(System, Processes) {
  MockParser parser;
  System system(parser);

  std::vector<int> pids{1, 38, 887, 2340, 3};

  // This stops set deduplication when sorting
  ON_CALL(parser, Pids()).WillByDefault(Return(pids));

  EXPECT_EQ(system.Processes().size(), 5);
  // Make sure doesn't change
  EXPECT_EQ(system.Processes().size(), 5);

  {
    std::vector<int> pids_expected = pids;
    std::vector<int> pids_found;
    for (auto p : system.Processes()) {
      pids_found.push_back(p.Pid());
    }

    std::sort(pids_expected.begin(), pids_expected.end());
    std::sort(pids_found.begin(), pids_found.end());

    EXPECT_THAT(pids_found, ::testing::ContainerEq(pids_expected));
  }
}