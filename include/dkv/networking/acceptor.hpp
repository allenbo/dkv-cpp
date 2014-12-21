#ifndef __DKV_NETWORKING_ACCEPTOR_HPP__
#define __DKV_NETWORKING_ACCEPTOR_HPP__

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "dkv/status.hpp"
#include "dkv/networking/channel.hpp"

namespace dkv { namespace networking {

class Acceptor {
  public:

    Acceptor(std::string addr);
    ~Acceptor();

    Status accept(Channel* ch);

  private:
    int socket_;
    struct addrinfo* addrinfo_;
};

} // networking
} // dkv

#endif
