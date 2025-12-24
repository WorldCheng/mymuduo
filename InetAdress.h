#pragma once

#include <arpa/inet.h>
#include <string>

class InetAdress {
public:
  explicit InetAdress(uint16_t port = 0, std::string ip = "127.0.0.1");
  InetAdress(const sockaddr_in &addr);
  const sockaddr *getSockAddr() const;
  std::string toIp() const;
  std::string toIpPort() const;
  uint16_t toPort() const;

private:
  sockaddr_in addr_;
};