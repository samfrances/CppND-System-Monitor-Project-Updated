#include "gmock/gmock.h"

#include <string>

#include "process.h"

using ::testing::Return;

class MockProcessParser : public ILinuxProcessParser {
 public:
  MOCK_METHOD(std::string, Command, (int pid), (const, override));
  MOCK_METHOD(std::string, Ram, (int pid), (const, override));
  MOCK_METHOD(std::string, Uid, (int pid), (const, override));
  MOCK_METHOD(std::string, User, (int pid), (const, override));
  MOCK_METHOD(long int, UpTime, (int pid), (const, override));
  MOCK_METHOD(long int, UpTime, (), (const, override));
  MOCK_METHOD(long, ActiveJiffies, (int pid), (const, override));
};

TEST(Process, Pid) {
  MockProcessParser parser;

  {
    Process p(parser, 5);
    EXPECT_EQ(p.Pid(), 5);
  }

  {
    Process p(parser, 49817);
    EXPECT_EQ(p.Pid(), 49817);
  }
}

TEST(Process, User) {
  {
    MockProcessParser parser;

    int pid = 10;
    std::string user = "bob";

    ON_CALL(parser, User(pid)).WillByDefault(Return(user));
    EXPECT_CALL(parser, User(pid)).Times(1);

    Process process(parser, pid);
    EXPECT_EQ(process.User(), user);
  }

  {
    MockProcessParser parser;

    int pid = 698;
    std::string user = "steve";

    ON_CALL(parser, User(pid)).WillByDefault(Return(user));
    EXPECT_CALL(parser, User(pid)).Times(1);

    Process process(parser, pid);
    EXPECT_EQ(process.User(), user);
  }
}

TEST(Process, Command) {
  {
    MockProcessParser parser;

    int pid = 5;
    std::string command = "foo --bar";

    ON_CALL(parser, Command(pid)).WillByDefault(Return(command));
    EXPECT_CALL(parser, Command(pid)).Times(1);

    Process process(parser, pid);
    EXPECT_EQ(process.Command(), command);
  }

  {
    MockProcessParser parser;

    int pid = 3245;
    std::string command = "/user/bin/blah --n 5";

    ON_CALL(parser, Command(pid)).WillByDefault(Return(command));
    EXPECT_CALL(parser, Command(pid)).Times(1);

    Process process(parser, pid);
    EXPECT_EQ(process.Command(), command);
  }
}

TEST(Process, Ram) {
  {
    MockProcessParser parser;

    int pid = 5;
    std::string ram = "34820.23";

    ON_CALL(parser, Ram(pid)).WillByDefault(Return(ram));
    EXPECT_CALL(parser, Ram(pid)).Times(1);

    Process process(parser, pid);
    EXPECT_EQ(process.Ram(), ram);
  }

  {
    MockProcessParser parser;

    int pid = 3245;
    std::string ram = "84749";

    ON_CALL(parser, Ram(pid)).WillByDefault(Return(ram));
    EXPECT_CALL(parser, Ram(pid)).Times(1);

    Process process(parser, pid);
    EXPECT_EQ(process.Ram(), ram);
  }
}

TEST(Process, UpTime) {
  {
    MockProcessParser parser;

    int pid = 84;
    long uptime = 883934;

    ON_CALL(parser, UpTime(pid)).WillByDefault(Return(uptime));
    EXPECT_CALL(parser, UpTime(pid)).Times(1);

    Process process(parser, pid);
    EXPECT_EQ(process.UpTime(), uptime);
  }

  {
    MockProcessParser parser;

    int pid = 3;
    long uptime = 99;

    ON_CALL(parser, UpTime(pid)).WillByDefault(Return(uptime));
    EXPECT_CALL(parser, UpTime(pid)).Times(1);

    Process process(parser, pid);
    EXPECT_EQ(process.UpTime(), uptime);
  }
}

TEST(Process, LessThan) {
  MockProcessParser parser;

  EXPECT_TRUE(Process(parser, 100) < Process(parser, 101));

  EXPECT_TRUE(Process(parser, 4) < Process(parser, 101));

  EXPECT_FALSE(Process(parser, 101) < Process(parser, 101));

  EXPECT_FALSE(Process(parser, 1010) < Process(parser, 101));
}

TEST(Process, CpuUtilization) {
  MockProcessParser parser;

  const int pid = 558;
  const long active_jiffies = 10000;
  const auto hertz = sysconf(_SC_CLK_TCK);
  const float total_time = (float)active_jiffies / (float)hertz;
  const long starttime = 100;
  const long uptime = 300;

  ON_CALL(parser, ActiveJiffies(pid)).WillByDefault(Return(active_jiffies));
  ON_CALL(parser, UpTime(pid)).WillByDefault(Return(starttime));
  ON_CALL(parser, UpTime()).WillByDefault(Return(uptime));

  EXPECT_CALL(parser, ActiveJiffies(pid)).Times(1);
  EXPECT_CALL(parser, UpTime(pid)).Times(1);
  EXPECT_CALL(parser, UpTime()).Times(1);

  Process process(parser, pid);

  const float expected = (total_time / (float)(uptime - starttime));
  EXPECT_EQ(process.CpuUtilization(), expected);
}