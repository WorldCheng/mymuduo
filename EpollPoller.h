#pragma once

#include "Poller.h"

#include <sys/epoll.h>
#include <vector>

/*
  epoll的使用
  1. epoll_create创建一个epoll实例
  2. epoll_ctl对实例进行操作 add/modify/delete
  3. epoll_wait监听epoll实例
*/

class EpollPoller : public Poller {
public:
  EpollPoller(EventLoop *loop);
  ~EpollPoller() override;

  Timestamp poll(int timeoutMs, ChannelList *activeChannels) override;
  // 对应epoll_ctl的操作
  void removeChannel(Channel *channel) override;
  void updateChannel(Channel *channel) override;

private:
  static const int kInitEventListSize = 16;

  // 根据epoll_wait返回的事件数量，填充活跃的Channel列表
  // numEvents: epoll_wait返回的事件数量
  // activeChannels: 输出参数，用于存储活跃的Channel列表
  void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;

  // 通用的epoll_ctl操作函数
  // operation: epoll操作类型(EPOLL_CTL_ADD, EPOLL_CTL_MOD, EPOLL_CTL_DEL)
  // channel: 需要操作的Channel
  void update(int operation, Channel *channel);

  using EvevtList = std::vector<epoll_event>;

  int epollfd_;
  EvevtList events_;
};