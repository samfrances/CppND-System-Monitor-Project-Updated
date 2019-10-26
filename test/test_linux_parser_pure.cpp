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