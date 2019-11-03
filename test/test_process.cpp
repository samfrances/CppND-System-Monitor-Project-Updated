#include "gmock/gmock.h"

#include "process.h"

using ::testing::Return;


TEST(Process, Pid) {
  {
    Process p(5);
    EXPECT_EQ(p.Pid(), 5);
  }

  {
    Process p(49817);
    EXPECT_EQ(p.Pid(), 49817);
  }
}