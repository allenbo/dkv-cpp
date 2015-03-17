#ifndef __DKV_CLIENT_HPP__
#define __DKV_CLIENT_HPP__

#include "dkv/status.hpp"
#include "dkv/request.hpp"
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

    /**
     * Sync update request
     */
    Status update(std::string& key, std::string& value);

    /**
     * Sync gets request
     */

    Status gets(std::vector<std::string>& keys,
                std::vector<std::string>& rkeys,
                std::vector<std::string>& rvalue);

    /**
     * Sync sets request
     */
    Status sets(std::vector<std::string>& keys,
                std::vector<std::string>& values,
                std::vector<std::string>& rkeys);
    
    /**
     * Sync dels request
     */
    Status dels(std::vector<std::string>& keys,
               std::vector<std::string>& rkeys);

    /**
     * Sync updates request
     */
    Status updates(std::vector<std::string>& keys,
                   std::vector<std::string>& values,
                   std::vector<std::string>& rkeys);

  private:

    Channel *ch_;

    Status multi_key_op(const Request& req,
                        std::vector<std::string>& rkeys);
    Status send_and_recv(const Buffer& send_buffer, Buffer& recv_buffer);
};


}

#endif
