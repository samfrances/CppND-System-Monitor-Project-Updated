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