#include "logger.h"
#include "Timestamp.h"

#include <iostream>

Logger &Logger::instance() {
  static Logger logger;
  return logger;
}

void Logger::setLogLevel(LogLevel level) { logLevel_ = level; }

#include <iostream> // 确保包含这个

void Logger::log(std::string_view message) {
  std::ostream *os = &std::cout; // 默认输出到 stdout

  switch (logLevel_) {
  case LogLevel::DEBUG:
    (*os) << "[DEBUG] ";
    break;

  case LogLevel::INFO:
    (*os) << "[INFO] ";
    break;

  case LogLevel::ERROR:
    (*os) << "[ERROR] ";
    break;

  case LogLevel::FATAL:
    (*os) << "[FATAL] ";
    break;
  }

  (*os) << TimeStamp::now(TimeStamp::Precision::Seconds) << ' ' << message
        << '\n';
}