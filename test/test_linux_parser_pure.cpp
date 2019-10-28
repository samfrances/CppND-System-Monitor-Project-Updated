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