#include <string>
#include <unistd.h>

#include "gmock/gmock.h"

#include "format.h"
#include "linux_parser_pure.h"

TEST(LinuxParserPureTests, TestOperatingSystem) {
  std::string etc_os_release =
    "NAME=\"Ubuntu\"\n"
    "VERSION=\"16.04.6 LTS (Xenial Xerus)\"\n"
    "ID=ubuntu\n"
    "ID_LIKE=debian\n"
    "PRETTY_NAME=\"Ubuntu 16.04.6 LTS\"\n"
    "VERSION_ID=\"16.04\"\n"
    "HOME_URL=\"http://www.ubuntu.com/\"\n"
    "SUPPORT_URL=\"http://help.ubuntu.com/\"\n"
    "BUG_REPORT_URL=\"http://bugs.launchpad.net/ubuntu/\"\n"
    "VERSION_CODENAME=xenial\n"
    "UBUNTU_CODENAME=xenial\n";

  std::istringstream contents_stream(etc_os_release);

  std::string result = LinuxParserPure::OperatingSystem(contents_stream);

  EXPECT_EQ(result, "Ubuntu 16.04.6 LTS");
}

TEST(LinuxParserPureTests, TestOperatingSystem_NotFound) {
  std::string etc_os_release =
    "NAME=\"Ubuntu\"\n"
    "VERSION=\"16.04.6 LTS (Xenial Xerus)\"\n"
    "ID=ubuntu\n"
    "ID_LIKE=debian\n"
          // "PRETTY_NAME=\"Ubuntu 16.04.6 LTS\"\n"
    "VERSION_ID=\"16.04\"\n"
    "HOME_URL=\"http://www.ubuntu.com/\"\n"
    "SUPPORT_URL=\"http://help.ubuntu.com/\"\n"
    "BUG_REPORT_URL=\"http://bugs.launchpad.net/ubuntu/\"\n"
    "VERSION_CODENAME=xenial\n"
    "UBUNTU_CODENAME=xenial\n";

  std::istringstream contents_stream(etc_os_release);

  std::string result = LinuxParserPure::OperatingSystem(contents_stream);

  EXPECT_EQ(result, "");
}

TEST(LinuxParserPureTests, TestKernel) {
  std::string proc_version =
    "Linux version 4.15.0-66-generic (buildd@lgw01-amd64-050) (gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.10)) #75~16.04.1-Ubuntu SMP Tue Oct 1 14:01:08 UTC 2019\n";

  std::istringstream contents_stream(proc_version);

  std::string result = LinuxParserPure::Kernel(contents_stream);

  EXPECT_EQ(result, "4.15.0-66-generic");
}

TEST(LinuxParserPureTests, TotalProcesses) {

  // Truncated example
  std::istringstream proc_stat_example_1(
    "ctxt 70733117\n"
    "processes 11102\n"
    "procs_running 15\n"
    "procs_blocked 0\n"
  );

  std::istringstream proc_stat_example_2(
    "ctxt 70733117\n"
    "processes 32\n"
    "procs_running 15\n"
    "procs_blocked 0\n"
  );


  EXPECT_EQ(
    LinuxParserPure::TotalProcesses(proc_stat_example_1),
    11102
  );

  EXPECT_EQ(
    LinuxParserPure::TotalProcesses(proc_stat_example_2),
    32
  );
}

TEST(LinuxParserPureTests, RunningProcesses) {

  // Truncated example
  std::istringstream proc_stat_example_1(
    "ctxt 70733117\n"
    "processes 11102\n"
    "procs_running 15\n"
    "procs_blocked 0\n"
  );

  std::istringstream proc_stat_example_2(
    "ctxt 70733117\n"
    "processes 11102\n"
    "procs_running 589\n"
    "procs_blocked 0\n"
  );


  EXPECT_EQ(
    LinuxParserPure::RunningProcesses(proc_stat_example_1),
    15
  );

  EXPECT_EQ(
    LinuxParserPure::RunningProcesses(proc_stat_example_2),
    589
  );
}

TEST(LinuxParserPureTests, UpTime) {

  std::istringstream proc_uptime_example_1("13145.60 43993.58");

  std::istringstream proc_uptime_example_2("2586.03 19468.30");

  {
    const long expected = 13145.60;
    EXPECT_EQ(
      LinuxParserPure::UpTime(proc_uptime_example_1),
      expected
    );
  }

  {
    const long expected = 2586.03;
    EXPECT_EQ(
      LinuxParserPure::UpTime(proc_uptime_example_2),
      expected
    );
  }

}

TEST(LinuxParserPureTests, MemoryUtilization) {

    // Incomplete example
  std::istringstream proc_meminfo(
    "MemTotal:        7910692 kB\n"
    "MemFree:          505260 kB\n"
    "MemAvailable:    2281140 kB\n"
    "Buffers:         1486140 kB\n"
    "Cached:          2617628 kB\n"
    "SwapCached:            0 kB\n"
    "Active:          5366276 kB\n"
    "Inactive:        1481720 kB\n"
  );

  const float total = 7910692;
  const float free = 505260;
  EXPECT_EQ(
    LinuxParserPure::MemoryUtilization(proc_meminfo),
    (total - free) / total
  );

}

TEST(LinuxParserPureTests, Command) {

  std::istringstream cmdline("/usr/bin/foo -d --bar\n");
  std::istringstream cmdline_blank("");

  EXPECT_EQ(
    LinuxParserPure::Command(cmdline),
    "/usr/bin/foo -d --bar"
  );

  EXPECT_EQ(
    LinuxParserPure::Command(cmdline_blank),
    ""
  );

}

class ProcStatusTest : public ::testing::Test {
protected:
  void SetUp() override {
    proc_status.str(
      "Name:   bash\n"
      "Umask:  0022\n"
      "State:  S (sleeping)\n"
      "Tgid:   17248\n"
      "Ngid:   0\n"
      "Pid:    17248\n"
      "PPid:   17200\n"
      "TracerPid:      0\n"
      "Uid:    1000    1000    1000    1000\n"
      "Gid:    100     100     100     100\n"
      "FDSize: 256\n"
      "Groups: 16 33 100\n"
      "NStgid: 17248\n"
      "NSpid:  17248\n"
      "NSpgid: 17248\n"
      "NSsid:  17200\n"
      "VmPeak:     3358960 kB\n"
      "VmSize:     3320716 kB\n"
      "VmLck:           0 kB\n"
      "VmPin:           0 kB\n"
      "VmHWM:       13484 kB\n"
      "VmRSS:       13484 kB\n"
      "RssAnon:     10264 kB\n"
      "RssFile:      3220 kB\n"
      "RssShmem:        0 kB\n"
      "VmData:      10332 kB\n"
      "VmStk:         136 kB\n"
      "VmExe:         992 kB\n"
      "VmLib:        2104 kB\n"
      "VmPTE:          76 kB\n"
      "VmPMD:          12 kB\n"
      "VmSwap:          0 kB\n"
      "HugetlbPages:          0 kB        # 4.4\n"
      "CoreDumping:   0                       # 4.15\n"
      "Threads:        1\n"
      "SigQ:   0/3067\n"
      "SigPnd: 0000000000000000\n"
      "ShdPnd: 0000000000000000\n"
      "SigBlk: 0000000000010000\n"
      "SigIgn: 0000000000384004\n"
      "SigCgt: 000000004b813efb\n"
      "CapInh: 0000000000000000\n"
      "CapPrm: 0000000000000000\n"
      "CapEff: 0000000000000000\n"
      "CapBnd: ffffffffffffffff\n"
      "CapAmb:   0000000000000000\n"
      "NoNewPrivs:     0\n"
      "Seccomp:        0\n"
      "Speculation_Store_Bypass:       vulnerable\n"
      "Cpus_allowed:   00000001\n"
      "Cpus_allowed_list:      0\n"
      "Mems_allowed:   1\n"
      "Mems_allowed_list:      0\n"
      "voluntary_ctxt_switches:        150\n"
      "nonvoluntary_ctxt_switches:     545\n"
    );
  }

  std::istringstream proc_status;
};

TEST_F(ProcStatusTest, Ram) {

  std::istringstream proc_status_incomplete(
    "Name:   bash\n"
    "Umask:  0022\n"
    "State:  S (sleeping)\n"
    "Tgid:   17248\n"
    "Ngid:   0\n"
    "Pid:    17248\n"
    "PPid:   17200\n"
    "TracerPid:      0\n"
    "Uid:    1000    1000    1000    1000\n"
    "Gid:    100     100     100     100\n"
  );

  EXPECT_EQ(
    LinuxParserPure::Ram(proc_status),
    std::to_string(3320716 / 1024.0)
  );

  EXPECT_EQ(
    LinuxParserPure::Ram(proc_status_incomplete),
    ""
  );

}

TEST_F(ProcStatusTest, Uid) {

  std::istringstream proc_status_incomplete(
    "VmRSS:       13484 kB\n"
    "RssAnon:     10264 kB\n"
    "RssFile:      3220 kB\n"
    "RssShmem:        0 kB\n"
    "VmData:      10332 kB\n"
    "VmStk:         136 kB\n"
    "VmExe:         992 kB\n"
    "VmLib:        2104 kB\n"
    "VmPTE:          76 kB\n"
    "VmPMD:          12 kB\n"
    "VmSwap:          0 kB\n"
  );

  EXPECT_EQ(
    LinuxParserPure::Uid(proc_status),
    "1000"
  );

  EXPECT_EQ(
    LinuxParserPure::Uid(proc_status_incomplete),
    ""
  );

}

TEST(LinuxParserPureTests, User) {

  std::string etc_passwd(
    "root:!:0:0::/:/usr/bin/ksh\n"
    "daemon:!:1:1::/etc:\n"
    "bin:!:2:2::/bin:\n"
    "sys:!:3:3::/usr/sys: \n"
    "adm:!:4:4::/var/adm:\n"
    "uucp:!:5:5::/usr/lib/uucp: \n"
    "guest:!:100:100::/home/guest:\n"
    "nobody:!:4294967294:4294967294::/:\n"
    "lpd:!:9:4294967294::/:\n"
    "lp:*:11:11::/var/spool/lp:/bin/false \n"
    "invscout:*:200:1::/var/adm/invscout:/usr/bin/ksh\n"
    "nuucp:*:6:5:uucp login user:/var/spool/uucppublic:/usr/sbin/uucp/uucico\n"
    "paul:!:201:1::/home/paul:/usr/bin/ksh\n"
    "jdoe:*:202:1:John Doe:/home/jdoe:/usr/bin/ksh\n"
  );

  {
    std::istringstream fstream(etc_passwd);
    EXPECT_EQ(
      LinuxParserPure::User(fstream, "202"),
      "jdoe"
    );
  }

  {
    std::istringstream fstream(etc_passwd);
    EXPECT_EQ(
      LinuxParserPure::User(fstream, "201"),
      "paul"
    );
  }

  {
    std::istringstream fstream(etc_passwd);
    EXPECT_EQ(
      LinuxParserPure::User(fstream, "203"),
      ""
    );
  }

}

TEST(LinuxParserPureTests, Uptime) {

  std::stringstream proc_pid_stat(
    "972 (docker-containe) S 901 972 972 0 -1 1077944576 2657 0 2 0 561 205 0 0 20 0 11 0 1820 441688064 2327 18446744073709551615 4194304 11049596 140727040242048 140727040241432 4602915 0 2079995941 0 2143420159 18446744073709551615 0 0 17 1 0 0 0 0 0 13147640 13322176 25554944 140727040249523 140727040249749 140727040249749 140727040249821 0"
  );

  EXPECT_EQ(
    LinuxParserPure::ProcessUpTime(proc_pid_stat),
    1820L / sysconf(_SC_CLK_TCK)
  );
}

TEST(PathTests, OperatingSystem) {

  EXPECT_EQ(
    LinuxParserPure::Paths::OperatingSystem(),
    "/etc/os-release"
  );

  EXPECT_EQ(
    LinuxParserPure::Paths::OperatingSystem("/foo/bar/"),
    "/foo/bar/etc/os-release"
  );
}

TEST(PathTests, Kernel) {

  EXPECT_EQ(
    LinuxParserPure::Paths::Kernel(),
    "/proc/version"
  );

  EXPECT_EQ(
    LinuxParserPure::Paths::Kernel("/foo/bar/"),
    "/foo/bar/proc/version"
  );
}

TEST(PathTests, Pids) {

  EXPECT_EQ(
    LinuxParserPure::Paths::Pids(),
    "/proc/"
  );

  EXPECT_EQ(
    LinuxParserPure::Paths::Pids("/foo/bar/"),
    "/foo/bar/proc/"
  );
}

TEST(PathTests, MemoryUtilization) {

  EXPECT_EQ(
    LinuxParserPure::Paths::MemoryUtilization(),
    "/proc/meminfo"
  );

  EXPECT_EQ(
    LinuxParserPure::Paths::MemoryUtilization("/foo/bar/"),
    "/foo/bar/proc/meminfo"
  );

}

TEST(PathTests, UpTime) {

  EXPECT_EQ(
    LinuxParserPure::Paths::UpTime(),
    "/proc/uptime"
  );

  EXPECT_EQ(
    LinuxParserPure::Paths::UpTime("/foo/bar/"),
    "/foo/bar/proc/uptime"
  );

}
