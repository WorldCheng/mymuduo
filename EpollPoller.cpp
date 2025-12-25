#include "EpollPoller.h"
#include "Logger.h"

#include <sys/epoll.h>
#include <unistd.h>

const int kNew = -1;  // 表示Channel是新创建的，还未添加到epoll中
const int kAdded = 1; // 表示Channel已经添加到epoll中进行监听
const int kDeleted = 2; // 表示Channel已经从epoll中删除，但对象仍然存在

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

void EpollPoller::fillActiveChannels(int numEvents,
                                     ChannelList *activeChannels) const {}
void EpollPoller::update(int operation, Channel *channel) {}