#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"

int main() {
  LinuxParser parser;
  System system(parser);
  NCursesDisplay::Display(system);
}