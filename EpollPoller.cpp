#include "EpollPoller.h"
#include "Logger.h"

#include <sys/epoll.h>
#include <unistd.h>

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

EpollPoller::EpollPoller(EventLoop *loop)
    : Poller(loop), epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
      events_(kInitEventListSize) {
  if (epollfd_ < 0) {
    LOG_FATAL("epoll_create1 error:%d \n", errno);
  }
}
EpollPoller::~EpollPoller() { ::close(epollfd_); }

Timestamp EpollPoller::poll(int timeoutMs, ChannelList *activeChannels) {}
void EpollPoller::removeChannel(Channel *channel) {}
void EpollPoller::updateChannel(Channel *channel) {}
