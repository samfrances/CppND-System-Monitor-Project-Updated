#include <string>

#include "gmock/gmock.h"

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

TEST(LinuxParserPure, MemoryUtilization) {

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