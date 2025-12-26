#include "Timestamp.h"

#include <chrono>
#include <format>

Timestamp Timestamp::now() {
  using namespace std::chrono;
  const auto tp = time_point_cast<microseconds>(system_clock::now());
  return Timestamp(static_cast<Rep>(tp.time_since_epoch().count()));
}

std::string Timestamp::toString() const {
  using namespace std::chrono;

  const auto tp =
      system_clock::time_point{microseconds{microsecondsSinceEpoch_}};
  const auto sec_tp = floor<seconds>(tp);
  const auto us_part = duration_cast<microseconds>(tp - sec_tp).count();

  std::time_t tt = system_clock::to_time_t(sec_tp);
  std::tm tm{};
#if defined(_WIN32)
  localtime_s(&tm, &tt); // 本地时区
                         // gmtime_s(&tm, &tt);    // UTC：想用就换这行
#else
  localtime_r(&tt, &tm);
  // gmtime_r(&tt, &tm);
#endif

  return std::format("{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:06}",
                     tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
                     tm.tm_min, tm.tm_sec, static_cast<int>(us_part));
}
