// TimeStamp.h
#pragma once

#include <string>

class Timestamp {
public:
  enum class Precision {
    Seconds,
    Milliseconds,
    Microseconds,
  };

  // 返回当前时间字符串（UTC）
  static std::string now(Precision p = Precision::Milliseconds);

  // 可自定义 chrono 格式（不带小数部分）
  // 例如 fmt = "{:%F %T}" 或 "{:%Y%m%d-%H%M%S}"
  static std::string now_custom(std::string_view fmt);
};
