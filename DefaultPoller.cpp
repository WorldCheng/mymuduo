#include "Poller.h"
#include <cstdlib>

Poller *Poller::newDefaultPoller(EventLoop *loop) {
  if (::getenv("MODUO_USE_POLL")) {
    return nullptr;
  } else {
    return nullptr;
  }
}