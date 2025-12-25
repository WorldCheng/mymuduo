#pragma once

#include <string_view>

#include "noncopyable.h"

#define LOG_INFO(logmsgFormat, ...)                                            \
  do {                                                                         \
    Logger &logger = Logger::instance();                                       \
    logger.setLogLevel(INFO);                                                  \
    char buf[1024];                                                            \
    std::snprintf(buf, sizeof(buf), logmsgFormat, ##__VA_ARGS__);              \
    logger.log(buf);                                                           \
  } while (0)

#define LOG_ERROR(logmsgFormat, ...)                                           \
  do {                                                                         \
    Logger &logger = Logger::instance();                                       \
    logger.setLogLevel(ERROR);                                                 \
    char buf[1024];                                                            \
    std::snprintf(buf, sizeof(buf), logmsgFormat, ##__VA_ARGS__);              \
    Logger.log(buf);                                                           \
  } while (0)

#define LOG_FATAL(logmsgFormat, ...)                                           \
  do {                                                                         \
    Logger &logger = Logger::instance();                                       \
    logger.setLogLevel(FATAL);                                                 \
    char buf[1024];                                                            \
    std::snprintf(buf, sizeof(buf), logmsgFormat, ##__VA_ARGS__);              \
    logger.log(buf);                                                           \
    exit(-1);                                                                  \
  } while (0)

#ifdef MUDEBUG
#define LOG_DEBUG(logmsgFormat, ...)                                           \
  do {                                                                         \
    Logger &logger = Logger::instance();                                       \
    logger.setLogLevel(DEBUG);                                                 \
    char buf[1024];                                                            \
    std::snprintf(buf, sizeof(buf), logmsgFormat, ##__VA_ARGS__);              \
    logger.log(buf);                                                           \
  } while (0)
#else
#define LOG_DEBUG(logmsgFormat, ...)
#endif

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