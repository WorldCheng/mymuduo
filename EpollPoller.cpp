#include "EpollPoller.h"
#include "Channel.h"
#include "Logger.h"
#include "Timestamp.h"

#include <cerrno>
#include <cstring>
#include <sys/epoll.h>
#include <unistd.h>

// channel的index_的值
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

Timestamp EpollPoller::poll(int timeoutMs, ChannelList *activeChannels) {
  LOG_INFO("func=%s => fd total count %d\n", __FUNCTION__,
           static_cast<int>(channels_.size()));
  int numEvents = ::epoll_wait(epollfd_, events_.data(),
                               static_cast<int>(events_.size()), timeoutMs);
  int savedError = errno;
  Timestamp now(Timestamp::now());
  if (numEvents > 0) {
    LOG_INFO("%d events happened\n", numEvents);
    fillActiveChannels(numEvents, activeChannels);
    if (static_cast<size_t>(numEvents) == events_.size()) {
      events_.resize(events_.size() * 2);
    }
  } else if (numEvents == 0) {
    LOG_DEBUG("%s timeout! \n", __FUNCTION__);
  } else {
    if (savedError != EINTR) {
      errno = savedError;
      LOG_ERROR("EPollPoller::poll():%d\n", savedError);
    }
  }
  return now;
}

/**
 * @brief 将epoll_wait返回的就绪事件填充到活动通道列表中
 *
 * 该函数遍历epoll_wait返回的就绪事件数组，从每个事件中提取出对应的Channel对象，
 * 设置Channel的就绪事件类型，然后将Channel添加到活动通道列表中。
 *
 * @param numEvents epoll_wait返回的就绪事件数量
 * @param activeChannels
 * 用于存储活动通道的列表，函数将就绪的Channel对象添加到此列表中
 */
void EpollPoller::fillActiveChannels(int numEvents,
                                     ChannelList *activeChannels) const {
  // 遍历所有就绪事件
  for (int i = 0; i < numEvents; ++i) {
    // 从epoll_event中取出Channel指针
    Channel *channel = static_cast<Channel *>(events_[i].data.ptr);
    // 设置Channel的就绪事件类型
    channel->set_revents(events_[i].events);
    // 将就绪的Channel添加到活动通道列表
    activeChannels->push_back(channel);
  }
}

// channel update/remove => EventLoop updateChannel => Poller updateChannel
void EpollPoller::removeChannel(Channel *channel) {
  int fd = channel->fd();
  int index = channel->index();
  channels_.erase(fd);
  LOG_INFO("func=%s fd=%d events=%d index=%d\n", __FUNCTION__, fd,
           channel->events(), index);
  if (index == kAdded) {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->set_index(kNew);
}

void EpollPoller::updateChannel(Channel *channel) {
  const int index = channel->index();
  LOG_INFO("func=%s fd=%d events=%d index=%d\n", __FUNCTION__, channel->fd(),
           channel->events(), index);
  if (index == kNew || index == kDeleted) {
    int fd = channel->fd();
    if (index == kNew) {
      channels_[fd] = channel;
    }
    channel->set_index(kAdded);
    update(EPOLL_CTL_ADD, channel);
  } else {
    if (channel->isNoneEvent()) {
      update(EPOLL_CTL_DEL, channel);
      channel->set_index(kDeleted);
    } else {
      update(EPOLL_CTL_MOD, channel);
    }
  }
}

/**
 * @brief 更新epoll中指定文件描述符的事件监听
 *
 * 该函数用于向epoll实例添加、修改或删除文件描述符的事件监听。
 *
 * @param operation
 * 操作类型，可以是EPOLL_CTL_ADD(添加)、EPOLL_CTL_MOD(修改)或EPOLL_CTL_DEL(删除)
 * @param channel 指向Channel对象的指针，包含需要监听的文件描述符和事件类型
 */
void EpollPoller::update(int operation, Channel *channel) {
  // 初始化epoll_event结构体，清零确保所有字段为默认值
  epoll_event event;
  memset(&event, 0, sizeof event);
  // 设置事件类型为channel中定义的事件
  event.events = channel->events();
  // 将channel指针存储在epoll_event的data.ptr字段，用于事件就绪时的回调
  event.data.ptr = channel;
  // 获取需要操作的文件描述符
  int fd = channel->fd();
  if (::epoll_ctl(epollfd_, operation, fd, &event) < 0) {
    if (operation == EPOLL_CTL_DEL) {
      LOG_ERROR("epoll_ctl del error:%d\n", errno);
    } else {
      LOG_FATAL("epoll_ctl add/mod error:%d\n", errno);
    }
  }
}
