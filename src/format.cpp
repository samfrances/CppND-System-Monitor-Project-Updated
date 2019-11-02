#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {

  long minutes = seconds / 60;
  seconds %= 60;
  long hours = minutes / 60;
  minutes %= 60;

  std::string hours_str = std::to_string(hours);
  // Overflow behaviour, for hours greater than 100
  // We opt for the y2k bug version :) for simplicity
  {
    const int len = hours_str.length();
    if (len > 2) {
      hours_str = hours_str.substr(len - 2, 2);
    }
  }
  const std::string minutes_str = std::to_string(minutes);
  const std::string seconds_str = std::to_string(seconds);

  const int n_zero = 2;
  const auto sep = ':';

  const std::string hours_trail = std::string(n_zero - hours_str.length(), '0');
  const std::string minutes_trail = std::string(n_zero - minutes_str.length(), '0');
  const std::string seconds_trail = std::string(n_zero - seconds_str.length(), '0');

  return hours_trail + hours_str + sep + minutes_trail + minutes_str + sep + seconds_trail + seconds_str;
}