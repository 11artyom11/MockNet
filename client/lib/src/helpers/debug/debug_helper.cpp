#include "../../../include/helpers/debug_helper/debug_helper.h"

/**
 * @brief gets current time ov working machine in unix style
 * used asctime_r instead of asctime in order to support multi-
 * thread debugging
 *
 * @return unix styled time converted into std::string
 */
std::string Debug::get_current_time(void) noexcept {
  time_t rawtime;
  struct tm* timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  char charbuf[30];
  asctime_r(timeinfo, charbuf);
  std::string timestr(charbuf);
  timestr.pop_back();
  return "[ " + timestr + " ]";
}
