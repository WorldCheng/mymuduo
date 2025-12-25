#pragma once

#include "Channel.h"
#include "noncopyable.h"

#include <unordered_map>
#include <vector>

// IO 复用
class Poller : noncopyable {
public:
  using ChannelList = std::vector<Channel *>; // 用于存储活跃的Channel列表

  Poller(EventLoop *);
  virtual ~Poller() = default;

  // 等待IO事件，超时时间为timeoutMs毫秒，将活跃的Channel存入activeChannels
  virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels) = 0;
  // 更新或添加Channel，将其注册到IO复用后端中
  virtual void updateChannel(Channel *) = 0;
  // 移除Channel，将其从IO复用后端中删除
  virtual void removeChannel(Channel *) = 0;

  // 检查Channel是否在Poller中
  bool hasChannel(Channel *) const;

  // 创建默认的Poller实例（通常是EPollPoller）
  static Poller *newDefaultPoller(EventLoop *);

protected:
  using ChannelMap = std::unordered_map<int, Channel *>; // fd到Channel的映射
  ChannelMap channels_; // 存储所有被管理的Channel

private:
  EventLoop *ownerLoop_; // 所属的EventLoop
};