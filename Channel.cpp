#include "Channel.h"
#include "Logger.h"
#include "Timestamp.h"

#include <sys/epoll.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1), tied_(false) {}

Channel::~Channel() {}

/**
 * @brief 将Channel与一个对象绑定，确保在对象生命周期内不会被销毁
 *
 * @param obj 用于绑定的shared_ptr对象，用来延长对象的生命周期
 */
void Channel::tie(const std::shared_ptr<void> &obj) {
  tie_ = obj;
  tied_ = true;
}

/**
 * @brief 更新Channel在EventLoop中的状态
 *
 * 该函数将当前Channel对象的状态更新到其关联的EventLoop中，
 * 使得EventLoop能够正确处理该Channel上的事件。
 */
void Channel::update() {
  // loop_->updateChannel(this);
}

void Channel::remove() {
  // loop_->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime) {
  // 检查是否与对象绑定，如果绑定了需要确保对象在处理事件期间不会被销毁
  if (tied_) {
    std::shared_ptr<void> guard = tie_.lock();
    if (guard) {
      // 如果成功获取到绑定对象的引用，安全地处理事件
      handleEventWithGuard(receiveTime);
    }
  } else {
    // 如果没有与对象绑定，直接处理事件
    handleEventWithGuard(receiveTime);
  }
}

/// <summary>
/// 根据epoll事件类型处理相应的回调函数
/// 此函数会根据revents_中的事件标志调用相应的回调函数
/// </summary>
/// <param name="receiveTime">事件接收时间戳</param>
void Channel::handleEventWithGuard(Timestamp receiveTime) {
  LOG_INFO("channel handleEvent revents:%d\n", revents_);

  // 检查是否收到EPOLLHUP事件且没有EPOLLIN事件，如果是则调用关闭回调函数
  if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
    if (closeCallback_) {
      closeCallback_();
    }
  }
  // 检查是否收到EPOLLERR错误事件，如果是则调用错误回调函数
  if (revents_ & EPOLLERR) {
    if (errorCallback_) {
      errorCallback_();
    }
  }
  // 检查是否收到EPOLLIN或EPOLLPRI事件（可读事件），如果是则调用读回调函数
  if (revents_ & (EPOLLIN | EPOLLPRI)) {
    if (readCallback_) {
      readCallback_(receiveTime);
    }
  }
  // 检查是否收到EPOLLOUT事件（可写事件），如果是则调用写回调函数
  if (revents_ & EPOLLOUT) {
    if (writeCallback_) {
      writeCallback_();
    }
  }
}