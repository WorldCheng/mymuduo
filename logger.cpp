#include "logger.h"
#include <print>

Logger &Logger::instance() {
  static Logger logger;
  return logger;
}

void Logger::setLogLevel(LogLevel level) { logLevel_ = level; }

void Logger::log(std::string_view message) {
  switch (logLevel_) {
  case LogLevel::DEBUG:
    std::print("[DEBUG]");
    break;
  case LogLevel::ERROR:
    std::print("[ERROR]");
    break;
  case LogLevel::FATAL:
    std::print("[FATAL]\n");
    break;
  case LogLevel::INFO:
    std::print("[INFO]\n");
  }
  std::print("print time {}\n", message);
}