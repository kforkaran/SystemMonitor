#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;

string Format::ElapsedTime(long seconds) {
  const int sph = 3600, spm = 60;

  long lseconds = seconds % spm, lminutes = (seconds / spm) % spm,
       lhours = seconds / sph;

  stringstream ssresult;
  ssresult << std::setfill('0') << std::setw(2) << lhours << ":";
  ssresult << std::setfill('0') << std::setw(2) << lminutes << ":";
  ssresult << std::setfill('0') << std::setw(2) << lseconds;
  return ssresult.str();
}
