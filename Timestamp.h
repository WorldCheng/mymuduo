// TimeStamp.h
#pragma once

#include <cstdint>
#include <string>

class Timestamp {
public:
  using Rep = std::int64_t;
  Timestamp() = default;
  explicit Timestamp(Rep us) : microsecondsSinceEpoch_(us) {}

  static Timestamp now();
  std::string toString() const;

private:
  Rep microsecondsSinceEpoch_ = 0;
};
