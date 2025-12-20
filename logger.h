#pragma once

#include <string_view>

#include "noncopyable.h"

enum LogLevel {
  INFO,
  ERROR,
  DEBUG,
  FATAL,
};

class Logger : noncopyable {
public:
  // 单例模式，只能创建一个对象
  static Logger &instance();
  void setLogLevel(LogLevel level);
  void log(std::string_view message);

private:
  int logLevel_;
  Logger() {}
};