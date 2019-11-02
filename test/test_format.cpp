#include "gmock/gmock.h"

#include "format.h"

TEST(Format, ElapsedTime) {
  EXPECT_EQ(Format::ElapsedTime(1), "00:00:01");
  EXPECT_EQ(Format::ElapsedTime(2), "00:00:02");
  EXPECT_EQ(Format::ElapsedTime(59), "00:00:59");
  EXPECT_EQ(Format::ElapsedTime(60), "00:01:00");
  EXPECT_EQ(Format::ElapsedTime(61), "00:01:01");
  EXPECT_EQ(Format::ElapsedTime(62), "00:01:02");
  EXPECT_EQ(Format::ElapsedTime(90), "00:01:30");
  EXPECT_EQ(Format::ElapsedTime(119), "00:01:59");
  EXPECT_EQ(Format::ElapsedTime(120), "00:02:00");
  EXPECT_EQ(Format::ElapsedTime(3540), "00:59:00");
  EXPECT_EQ(Format::ElapsedTime(3599), "00:59:59");
  EXPECT_EQ(Format::ElapsedTime(3600), "01:00:00");
  EXPECT_EQ(Format::ElapsedTime(3600 * 2), "02:00:00");
  EXPECT_EQ(Format::ElapsedTime(3600 * 3), "03:00:00");
  EXPECT_EQ(Format::ElapsedTime(3600 * 60), "60:00:00");
  EXPECT_EQ(Format::ElapsedTime(3600 * 61), "61:00:00");
  EXPECT_EQ(Format::ElapsedTime(3600 * 99), "99:00:00");
  // What happens when it reaces the limit of the format?
  EXPECT_EQ(Format::ElapsedTime(3600 * 100), "00:00:00");
  EXPECT_EQ(Format::ElapsedTime(3600 * 101), "01:00:00");
}