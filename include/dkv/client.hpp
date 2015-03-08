#ifndef __DKV_CLIENT_HPP__
#define __DKV_CLIENT_HPP__

#include "dkv/status.hpp"
#include "networking/channel.hpp"
#include "networking/buffer.hpp"
#include "common/all.hpp"

namespace dkv {

using networking::Buffer;
using networking::Channel;

class Client {
  public:
    Client(std::string addr);

    /**
     * Sync get request
     */
    Status get(std::string& key, std::string* value);

    /**
     * Sync del request
     */
    Status del(std::string& key);

    /**
     * Sync set request
     */
    Status set(std::string& key, std::string& value);

  private:
    Channel *ch_;
};


}

#endif
