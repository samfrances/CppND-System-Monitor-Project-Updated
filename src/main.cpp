#include "linux_parser.h"
#include "ncurses_display.h"
#include "system.h"

int main() {
  LinuxParser parser(
      "/");  // TODO see if we can do without specifying the root here
  System system(parser);
  NCursesDisplay::Display(system);
}