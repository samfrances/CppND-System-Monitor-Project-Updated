#include "linux_parser.h"
#include "ncurses_display.h"
#include "system.h"

int main() {
  LinuxParser parser;
  System system(parser);
  NCursesDisplay::Display(system);
}