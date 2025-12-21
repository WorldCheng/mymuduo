#include "Timestamp.h"

#include <chrono>
#include <format>
#include <string_view>

// 返回当前时间字符串（UTC）
std::string TimeStamp::now(Precision p) {
  using namespace std::chrono;
  const auto tp = system_clock::now();
  switch (p) {
  case Precision::Seconds: {
    return std::format("{:%F %T}", floor<seconds>(tp));
  }
  case Precision::Milliseconds: {
    auto sec = floor<seconds>(tp);
    auto ms = duration_cast<milliseconds>(tp - sec).count();
    return std::format("{:%F %T}.{:03}", sec, ms);
  }
  case Precision::Microseconds: {
    auto sec = floor<seconds>(tp);
    auto us = duration_cast<microseconds>(tp - sec).count();
    return std::format("{:%F %T}.{:06}", sec, us);
  }
  }

  return {}; // 理论不会到这
}

// 可自定义 chrono 格式（不带小数部分）
// 例如 fmt = "{:%F %T}" 或 "{:%Y%m%d-%H%M%S}"
std::string TimeStamp::now_custom(std::string_view fmt) {
  using namespace std::chrono;
  auto tp = floor<seconds>(system_clock::now());

  // 关键：vformat + make_format_args
  return std::vformat(fmt, std::make_format_args(tp));
}
