#include "gmock/gmock.h"

#include "system.h"
#include "linux_parser.h"
#include "linux_parser_pure.h"

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
  MOCK_METHOD(LinuxParserPure::CpuUtilizationSnapshot, CpuUtilization, (), (const, override));
  MOCK_METHOD(long, Jiffies, (), (const, override));        // TODO consider deleting
  MOCK_METHOD(long, ActiveJiffies, (), (const, override));  // TODO consider deleting
  MOCK_METHOD(long, ActiveJiffies, (int pid), (const, override));
  MOCK_METHOD(long, IdleJiffies, (), (const, override));  // TODO consider deleting

  // Processes
  MOCK_METHOD(std::string, Command, (int pid), (const, override));
  MOCK_METHOD(std::string, Ram, (int pid), (const, override));
  MOCK_METHOD(std::string, Uid, (int pid), (const, override));
  MOCK_METHOD(std::string, User, (int pid), (const, override));
  MOCK_METHOD(long int, UpTime, (int pid), (const, override));
};

TEST(System, MemoryUtilization) {
  MockParser parser;
  System system(parser);

  std::vector<float> expectations{0.72, 0.1, 1.0, 0};

  for (const float expected: expectations) {

    ON_CALL(parser, MemoryUtilization())
    .WillByDefault(Return(expected));

    EXPECT_CALL(parser, MemoryUtilization())
      .Times(1);

    EXPECT_EQ(system.MemoryUtilization(), expected);

  }

}