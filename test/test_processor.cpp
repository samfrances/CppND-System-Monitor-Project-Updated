#include "gmock/gmock.h"

#include "linux_parser.h"
#include "linux_parser_pure.h"
#include "processor.h"

using ::testing::Return;

class MockProcessorParser : public ILinuxProcessorParser {
 public:
  MOCK_METHOD(LinuxParserPure::CpuUtilizationSnapshot, CpuUtilization, (),
              (const, override));
  MOCK_METHOD(long, Jiffies, (), (const, override));  // TODO consider deleting
  MOCK_METHOD(long, ActiveJiffies, (),
              (const, override));  // TODO consider deleting
  MOCK_METHOD(long, ActiveJiffies, (int pid), (const, override));
  MOCK_METHOD(long, IdleJiffies, (),
              (const, override));  // TODO consider deleting
};

TEST(Processor, Utilization) {
  MockProcessorParser parser;
  Processor proc(parser);

  // Bootstrap
  LinuxParserPure::CpuUtilizationSnapshot snapshot(3434,  // user
                                                   4325,  // nice
                                                   44,    // system
                                                   35,    // idle
                                                   6567,  // iowait
                                                   342,   // irq
                                                   543,   // softirq
                                                   678    // steal
                                                   );

  // 50 % utilisation
  LinuxParserPure::CpuUtilizationSnapshot snapshot2(
      snapshot.user + 100,  // user
      snapshot.nice,        // nice
      snapshot.system,      // system
      snapshot.idle + 100,  // idle
      snapshot.iowait,      // iowait
      snapshot.irq,         // irq
      snapshot.softirq,     // softirq
      snapshot.steal        // steal
      );

  // Attempted backwards step, where total goes backwards
  LinuxParserPure::CpuUtilizationSnapshot snapshot3(
      snapshot2.user - 100,  // user
      snapshot2.nice,        // nice
      snapshot2.system,      // system
      snapshot2.idle,        // idle
      snapshot2.iowait,      // iowait
      snapshot2.irq,         // irq
      snapshot2.softirq,     // softirq
      snapshot2.steal        // steal
      );

  // Attempted backwards step, where only active goes backwards
  LinuxParserPure::CpuUtilizationSnapshot snapshot4(
      snapshot2.user - 100,  // user
      snapshot2.nice,        // nice
      snapshot2.system,      // system
      snapshot2.idle + 100,  // idle
      snapshot2.iowait,      // iowait
      snapshot2.irq,         // irq
      snapshot2.softirq,     // softirq
      snapshot2.steal        // steal
      );

  // Another forward sample
  LinuxParserPure::CpuUtilizationSnapshot snapshot5(
      snapshot2.user + 70,  // user
      snapshot2.nice,       // nice
      snapshot2.system,     // system
      snapshot2.idle + 30,  // idle
      snapshot2.iowait,     // iowait
      snapshot2.irq,        // irq
      snapshot2.softirq,    // softirq
      snapshot2.steal       // steal
      );

  // Identical snapshot
  auto snapshot6 = snapshot5;

  // No active increase
  LinuxParserPure::CpuUtilizationSnapshot snapshot7(
      snapshot6.user,         // user
      snapshot6.nice,         // nice
      snapshot6.system,       // system
      snapshot6.idle + 50,    // idle
      snapshot6.iowait + 12,  // iowait
      snapshot6.irq,          // irq
      snapshot6.softirq,      // softirq
      snapshot6.steal         // steal
      );

  // 100% active
  LinuxParserPure::CpuUtilizationSnapshot snapshot8(
      snapshot7.user + 5,    // user
      snapshot7.nice + 10,   // nice
      snapshot7.system + 1,  // system
      snapshot7.idle,        // idle
      snapshot7.iowait,      // iowait
      snapshot7.irq,         // irq
      snapshot7.softirq,     // softirq
      snapshot7.steal        // steal
      );

  EXPECT_CALL(parser, CpuUtilization())
      .WillOnce(Return(snapshot))
      .WillOnce(Return(snapshot2))
      .WillOnce(Return(snapshot3))
      .WillOnce(Return(snapshot4))
      .WillOnce(Return(snapshot5))
      .WillOnce(Return(snapshot6))
      .WillOnce(Return(snapshot7))
      .WillOnce(Return(snapshot8));

  // Bootstrap
  EXPECT_EQ(proc.Utilization(), 0);
  // 50%
  EXPECT_NEAR(proc.Utilization(), 0.5, 0.01);
  // Failed backwards step
  EXPECT_NEAR(proc.Utilization(), 0.5, 0.01);
  // Failed backwards step (in active)
  EXPECT_NEAR(proc.Utilization(), 0.5, 0.01);
  // Forward again 70%
  EXPECT_NEAR(proc.Utilization(), 0.7, 0.01);
  // Identical snapshot
  EXPECT_NEAR(proc.Utilization(), 0.7, 0.01);
  // No active time
  EXPECT_NEAR(proc.Utilization(), 0, 0.01);
  // 100% active time
  EXPECT_NEAR(proc.Utilization(), 1, 0.01);
}