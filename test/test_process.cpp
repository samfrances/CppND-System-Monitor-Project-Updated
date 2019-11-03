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

TEST(Process, Command) {
  {
    MockProcessParser parser;

    int pid = 5;
    std::string command = "foo --bar";

    ON_CALL(parser, Command(pid)).WillByDefault(Return(command));

    Process process(parser, pid);
    EXPECT_EQ(process.Command(), command);
  }

  {
    MockProcessParser parser;

    int pid = 3245;
    std::string command = "/user/bin/blah --n 5";

    ON_CALL(parser, Command(pid)).WillByDefault(Return(command));

    Process process(parser, pid);
    EXPECT_EQ(process.Command(), command);
  }
}