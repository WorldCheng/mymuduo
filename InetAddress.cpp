#include "InetAdress.h"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <print>

InetAdress::InetAdress(uint16_t port, std::string ip) {
  std::memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);
  ::inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr);
}

InetAdress::InetAdress(const sockaddr_in &addr) : addr_(addr) {}
const sockaddr *InetAdress::getSockAddr() const {
  return reinterpret_cast<const sockaddr *>(&addr_);
}
std::string InetAdress::toIp() const {
  char buf[INET_ADDRSTRLEN];
  ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
  return std::string(buf);
}

std::string InetAdress::toIpPort() const {
  char buf[64];
  ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
  auto end = strlen(buf);
  snprintf(buf + end, sizeof(buf) - end, ":%u", ntohs(addr_.sin_port));
  std::string s =
      std::format("{}:{}", InetAdress::toIp(), InetAdress::toPort());
  // return std::string(buf);
  return s;
}
uint16_t InetAdress::toPort() const { return ntohs(addr_.sin_port); }

int main() {
  InetAdress add(8080, "223.12.5.7");
  std::println("{}", add.toIpPort());
  std::println("{}", add.toIp());
  std::println("{}", add.toPort());

  return 0;
}